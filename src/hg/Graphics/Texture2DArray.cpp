#include "Texture2DArray.hpp"
#include "hd/Core/Log.hpp"
#include "hd/Core/StringUtils.hpp"

namespace hg {

Texture2DArray::Texture2DArray(uint32_t id, const glm::ivec2 &size, TextureFormat format, uint32_t layers) : Texture(id, format, TextureType::Tex2D) {
    mSize = size;
    mLayers = layers;
}

const glm::ivec2 &Texture2DArray::getSize() const {
    return mSize;
}

uint32_t Texture2DArray::getLayers() const {
    return mLayers;
}

Texture2DArrayPtr Texture2DArray::create(const void **data, uint32_t layers, const glm::ivec2 &size, TextureFormat format) {
    uint32_t id;
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &id);
    glTextureStorage3D(id, 1, mGetTextureInternalFormat(format), size.x, size.y, layers);
    for (uint32_t i = 0; i < layers; i++) {
        glTextureSubImage3D(id, 0, 0, 0, i, size.x, size.y, 1, mGetTextureExternalFormat(format), mGetTextureDataType(format), data[i]);
    }
    glGenerateTextureMipmap(id);

    return std::make_shared<Texture2DArray>(id, size, format, layers);
}

Texture2DArrayPtr Texture2DArray::createFromImages(const std::vector<hd::Image> &images) {
    TextureFormat format = mGetTextureFormatFromImageFormat(images.front().getFormat());
    glm::ivec2 size = images.front().getSize();
    uint32_t layers = images.size();

    std::vector<const void*> data;
    for (const auto &image : images) {
        data.push_back(image.getData());
    }

    return create(data.data(), layers, size, format);
}

Texture2DArrayPtr Texture2DArray::createFromFiles(const std::vector<std::string> &paths) {
    if (paths.empty()) {
        HD_LOG_FATAL("No images paths to load into Texture2DArray");
    }

    std::vector<hd::Image> images;
    images.reserve(paths.size());
    for (const auto &path : paths) {
        hd::Image image = hd::Image(mGetFullPath(path), hd::ImageFormat::None, false);
        images.push_back(image);
        if (images.size() > 1 && (images.front().getSize() != images.back().getSize() || images.front().getFormat() != images.back().getFormat())) {
            HD_LOG_FATAL("Failed to create Texture2DArray from files:\n{}\nImages must be same size and same format", hd::StringUtils::unite(paths, "'", "'", "\n"));
        }
    }
    return createFromImages(images);
}

}
