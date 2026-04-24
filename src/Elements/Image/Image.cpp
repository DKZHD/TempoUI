#include "Image.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include "UIMesh/UIMesh.h"
#include "ImageHandler/ImageHandler.h"

ImageConfig& ImageConfig::texture_name(const std::string& name)
{
    tex_name_ = name;
    return *this;
}

Image::Image(const ImageConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex()), tex_name_(_config.texture_name())
{
    config.localPosition = _config.position();
    config.size = _config.size();
    mesh = std::make_unique<UIMesh>(get_position(), _config.size(), glm::vec4(1.f), glm::vec4(0.f), ImageHandler::get().get_texture(_config.texture_name()));
}

Image::Image(const std::string& id, std::string tex_name, glm::vec2 pos, glm::vec2 size) : ElementBase(id), tex_name_(std::move(tex_name))
{
    config.localPosition = pos;
    config.size = size;
    mesh = std::make_unique<UIMesh>(get_position(), size, glm::vec4(1.f), glm::vec4(0.f), ImageHandler::get().get_texture(tex_name_));
}

Image::~Image()
{
}

void Image::set_texture(const std::string& name)
{
    tex_name_ = name;
    mark_dirty();
}

void Image::update_buffers()
{
    if (parent != nullptr)
    {
        mesh->update_position(get_position());
    }

    vertices.clear();
    indices.clear();

    mesh->update_size(config.size);

    mesh->update_texture_id(ImageHandler::get().get_texture(tex_name_));

    vertices.reserve(4);
    indices.reserve(6);

    for (unsigned int index : mesh->get_indices())
    {
        indices.push_back(index + vertices.size());
    }
    vertices.insert(vertices.end(), mesh->get_vertices().begin(), mesh->get_vertices().end());

    for (const auto& child : children)
    {
        child->update_absolute_position();
        child->update_buffers();
    }
}

static LayoutRegistry::AutoRegister dummy("Image", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
{
    ImageConfig config;
    config.id(props.Id.value());
    if (props.Pos)
        config.position(props.Pos.value());
    if (props.Size)
        config.size(props.Size.value());
    if (props.Radius)
        config.radius(props.Radius.value());
    if (props.Flex)
        config.flex(props.Flex.value());

    if (auto it = json_object.find("Texture"); it != json_object.end())
    {
        config.texture_name(it->second.as_string());
    }
    return std::make_unique<Image>(config);
});