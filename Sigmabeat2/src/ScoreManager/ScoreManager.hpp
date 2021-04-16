#pragma once
#include "../Common.hpp"

namespace Score {

    struct Data {
        String title;
        String artist;
        String category;
        String url;
        FilePath path;
        FilePath musicPath;
        FilePath imagePath;


        Color backgroundColor[4];

    };

    class Manager {
    public:

    private:
        Array<Data> m_scores;
    };
}