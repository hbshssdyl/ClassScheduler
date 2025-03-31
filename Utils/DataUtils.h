#pragma once

#include <String>
#include <vector>

using namespace std;

namespace PictureManager
{
    struct ImageInfo
    {
        int width;
        int height;
        int channel;
        string name;
        string path;
        bool isReadyToDelete;
    };
    using ImagesInfo = vector<ImageInfo>;

    struct RepeatedImages
    {
        ImagesInfo images;
        int maxWidth;
        int maxHeight;
    };
} // namespace PictureManager
