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
        int32 offsetMs;
        double bpm;
        Color backgroundColor[4];
        uint32 level[4];

        size_t noteStartSeek;
        bool isValid;
    };

    class Manager {
    public:
        Manager();
        ~Manager();
        
        void init(const FilePath &scoreDirectory = U"scores/");
        bool load();
        bool debugPrint(size_t index);

    private:
        bool setScoreData(Data &score, const String &tagname, const String &tagvalue);

        bool m_hasInitialized;
        bool m_hasLoaded;
        Array<Data> m_scores;
    };
}