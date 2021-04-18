#pragma once
#include "../Common.hpp"

namespace Score {

    struct Data {
        String title;
        String artist;
        String category;
        String url;
        FilePath path;
        FilePath parentPath;
        FilePath musicPath;
        FilePath imagePath;
        
        uint32 demoStartMs;
        uint32 offsetMs;
        double bpm;
        Color backgroundColor[4];

        size_t noteStartSeek;
        bool isValid;
    };

    class Manager {
    public:
        Manager();
        ~Manager();
        
        void init(const FilePath &scoreDirectory = U"scores/");
        bool load();


    private:
        bool m_hasInitialized;
        bool m_hasLoaded;
        Array<Data> m_scores;
    };
}