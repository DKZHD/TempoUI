#include "UIRenderer.h"
#include "DragDropUI/DragDropUI.h"
#include "Elements/Canvas/Canvas.h"
#include "Shader/DefaultShaders.h"
#include "Shader/Shader.h"
#include "UIMesh/UIMesh.h"
#include "AnimationManager/AnimationManager.h"
#include "ThemeManager/ThemeManager.h"
#include "EventManager/EventManager.h"
#include "IRenderer/IRenderer.h"
#include "IRenderer/CustomRenderers/OpenGL/OpenGLRenderer.h"
#include "LayoutLoader/LayoutLoader.h"
#include <queue>

namespace TempoUI
{
#ifdef GLFW_UI
#include <GLFW/glfw3.h>
#endif

    namespace
    {
        std::queue<ElementBase*> overlays;
    } // namespace

    UIRenderer::UIRenderer(const char* theme_data)
    {
        if (theme_data != nullptr)
        {
            ThemeManager::get().load_theme(theme_data);
        }
        shader = std::make_unique<Shader>(DefaultShaders::vertex_shader, DefaultShaders::fragment_shader, false);
        backend_renderer = std::make_unique<OpenGLRenderer>(*shader);
        backend_renderer->init(nullptr);

        EventManager::get().bind_char_event([this](char c)
        {
            canvas->on_char(c);
        });

        EventManager::get().bind_scroll_event([this](double y_offset)
        {
            canvas->on_scroll((float)y_offset);
        });

        EventManager::get().bind_mouse_button_event([this](UI::MouseButton button, UI::Action action)
        {
            if (!DragDropUI::s_is_moveable)
            {
                canvas->on_mouse_button(button, action);
                return;
            }

            if (action == UI::Action::Press && button == UI::MouseButton::Left)
            {
                if (std::unique_ptr<ElementBase> element = canvas->release_hovered_element())
                {
                    DragDropUI::begin_drag_drop(std::move(element));
                }
            }
            if (action == UI::Action::Release && button == UI::MouseButton::Left)
            {
                if (ElementBase* hovered = canvas->get_hovered_element())
                {
                    std::unique_ptr<ElementBase> dropped_element = DragDropUI::end_drag_drop();

                    while (hovered != nullptr && !hovered->check_state(IS_CONTAINER))
                    {
                        hovered = hovered->get_parent();
                    }
                    if (hovered)
                    {

                        glm::vec2 element_abs_pos = dropped_element->get_position();
                        glm::vec2 new_parent_abs_pos = hovered->get_position();
                        dropped_element->set_local_position(element_abs_pos - new_parent_abs_pos);

                        hovered->add_child(std::move(dropped_element));
                        hovered->mark_dirty();
                    }
                    else
                    {
                        hovered->add_child(std::move(dropped_element));
                    }
                }
            }
        });

        EventManager::get().bind_key_event([this](UI::Key key, UI::Action action)
        {
            canvas->on_key(key, action);
        });
    }

    UIRenderer::~UIRenderer()
    {
    }

    void UIRenderer::init(float width, float height)
    {
        canvas = std::make_unique<Canvas>("Screen", glm::vec2(0.f, 0.f), glm::vec2(width, height));
    }

    bool UIRenderer::draw(float delta_time)
    {
        update(delta_time);

        bool is_dirty = canvas->get_is_dirty();
        if (is_dirty)
        {
            backend_renderer->begin_frame();
            render();
            backend_renderer->update_buffers(vertices, indices);
            for (const auto& batch : render_batches_)
            {
                if (batch.use_scissor)
                {
                    backend_renderer->set_scissor(
                        (int)batch.scissor_area.x,
                        (int)batch.scissor_area.y,
                        (int)batch.scissor_area.z,
                        (int)batch.scissor_area.w,
                        (int)canvas->get_size().y);
                }
                else
                {
                    backend_renderer->disable_scissor();
                }

                backend_renderer->draw_indexed(batch.start_index, batch.index_count);
            }
            backend_renderer->disable_scissor();
            render_batches_.clear();
            backend_renderer->end_frame();
        }
        return is_dirty;
    }

    void UIRenderer::update(float delta_time)
    {
        AnimationManager::get().update(delta_time);
        if (DragDropUI::s_is_moveable)
        {
            DragDropUI::handle_drag_drop();
            canvas->mark_dirty();
        }

        canvas->update(delta_time);

        ElementBase* element = canvas->get_hovered_overlay();
        if (element == nullptr)
        {
            element = canvas->get_hovered_element();
        }

        if (element != last_hovered)
        {
            if (last_hovered != nullptr)
            {
                last_hovered->on_unhover();
            }
            if (element != nullptr)
            {
                element->on_hover();
            }

            last_hovered = element;
        }
    }

    void UIRenderer::render()
    {
        shader->use();
        const glm::mat4 projection = glm::ortho(0.f, canvas->get_size().x, canvas->get_size().y, 0.f);
        shader->set_mat4("Projection", projection);

        shader->load_images();

        if (canvas->get_is_dirty())
        {
            std::vector<std::unique_ptr<ElementBase>>& children = canvas->get_children();

            vertices.clear();
            vertices.reserve(1000);

            indices.clear();
            indices.reserve(1500);

            glm::vec2 screen_size = canvas->get_size();

            glm::vec4 current_clip = glm::vec4(0.f, 0.f, screen_size.x, screen_size.y);

            render_batches_.emplace_back(0, 0, true, current_clip);

            process_elements(canvas.get(), current_clip);

            while (!overlays.empty())
            {
                ElementBase* overlay = overlays.front();
                overlay->set_state(IS_OVERLAY, false);
                process_elements(overlay, glm::vec4(0.f, 0.f, screen_size.x, screen_size.y));
                overlay->set_state(IS_OVERLAY, true);
                overlays.pop();
            }

            if (DragDropUI::s_is_moveable)
            {
                DragDropUI::ElementInfo dragged = DragDropUI::get_element_info();

                glm::vec4 screen_clip(0.f, 0.f, screen_size.x, screen_size.y);
                render_batches_.emplace_back(indices.size(), 0, true, screen_clip);

                for (auto& index : dragged.indices_)
                {
                    indices.emplace_back(index + vertices.size());
                    render_batches_.back().index_count++;
                }
                vertices.insert(vertices.end(), dragged.vertices_.begin(), dragged.vertices_.end());
            }
            canvas->clear_dirty();
        }
    }

    void UIRenderer::add_image(const std::string& path)
    {
        shader->add_image(path);
    }

    void UIRenderer::add_image(const std::string& name, const std::string& path)
    {
        shader->add_texture(name, path);
    }

    void UIRenderer::add_image_from_memory(const std::string& name, unsigned char* data, int length)
    {
        shader->add_image_from_memory(name, data, length);
    }

    void UIRenderer::add_font(const std::string& name, const std::string& path, unsigned int font_size)
    {
        uint32_t atlas = shader->load_font(name, path, font_size);
        shader->add_texture(name, atlas);
    }

    void UIRenderer::add_font_from_memory(const std::string& name, unsigned char* data, long length, unsigned int font_size)
    {
        unsigned int atlas = shader->load_font_from_memory(name, data, length, font_size);
        shader->add_texture(name, atlas);
    }

#ifdef LAYOUT_LOADER
    void UIRenderer::bind_layout_callback(std::function<void()> func)
    {
        layout_callback_ = std::make_unique<std::function<void()>>(std::move(func));
    }

    void UIRenderer::load_layout(const std::string& layout_name)
    {
        canvas->add_child(std::move(LayoutLoader::load_layout(layout_name)));
        if (layout_callback_)
        {
            (*layout_callback_)();
        }
    }
#endif

    void UIRenderer::remove_element_from_canvas(const std::string& id)
    {
        if (last_hovered)
        {
            last_hovered->on_unhover();
            last_hovered = nullptr;
        }
        canvas->remove_element(id);
    }

    void UIRenderer::process_elements(ElementBase* element, glm::vec4 active_clipping_area)
    {
        if (!element->check_state(VISIBLE))
            return;

        if (element->check_state(IS_OVERLAY))
        {
            overlays.emplace(element);
            return;
        }

        if (active_clipping_area != render_batches_.back().scissor_area)
        {
            if (render_batches_.back().index_count == 0)
            {
                render_batches_.back().scissor_area = active_clipping_area;
            }
            else
            {
                render_batches_.emplace_back(indices.size(), 0, true, active_clipping_area);
            }
        }

        for (unsigned int index : element->get_indices())
        {
            indices.push_back(index + vertices.size());
            render_batches_.back().index_count++;
        }

        std::vector<Vertex>& element_vertices = element->get_vertices();
        vertices.insert(vertices.end(), element_vertices.begin(), element_vertices.end());

        glm::vec4 children_clip = active_clipping_area;

        if (element->check_state(CLIP_CHILDREN))
        {
            glm::vec2 pos = element->get_position();
            glm::vec2 size = element->get_size();

            float x1 = std::max(active_clipping_area.x, pos.x);
            float y1 = std::max(active_clipping_area.y, pos.y);
            float x2 = std::min(active_clipping_area.x + active_clipping_area.z, pos.x + size.x);
            float y2 = std::min(active_clipping_area.y + active_clipping_area.w, pos.y + size.y);

            children_clip = glm::vec4(x1, y1, std::max(0.f, x2 - x1), std::max(0.f, y2 - y1));
        }
        for (const std::unique_ptr<ElementBase>& child : element->get_children())
        {
            process_elements(child.get(), children_clip);
        }
    }
} // namespace TempoUI
