#pragma once
#include "Elements/ElementBase/ElementBase.h"

class UIMesh;

class ImageConfig : public ElementConfig<ImageConfig>
{
public:
    ImageConfig& texture_name(const std::string& name);
    [[nodiscard]] const std::string& texture_name() const { return tex_name_; }

private:
    std::string tex_name_;
};

class Image : public ElementBase
{
public:
    Image(const ImageConfig& _config);
    Image(const std::string& id, std::string tex_name, glm::vec2 pos, glm::vec2 size);
    ~Image();

    void set_texture(const std::string& name);

    std::string tex_name_;

private:
    void update_buffers() override;

    std::unique_ptr<UIMesh> mesh;
};
