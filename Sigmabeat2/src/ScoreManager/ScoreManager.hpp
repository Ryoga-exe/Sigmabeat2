#pragma once
#include "../Common.hpp"

namespace Score {

    const String unvalid = U"unvalid";

    struct Data {
        String title;
        String artist;
        String category;
        String url;
        FilePath path;
        FilePath parentPath;
        FilePath musicPath;
        FilePath imagePath;
        int32 priority = 0;
        Texture texture;

        uint32 demoStartMs;
        int32 offsetMs;
        double bpm;
        Color backgroundColor[4];
        uint32 level[4];

        size_t noteStartSeek;
        bool isValid = false;
    };

    class Manager {
    public:
        Manager();
        ~Manager();
        
        void init(const FilePath &scoreDirectory = U"scores/");
        bool load();

        size_t getScoreSize();
        const Texture& getTexture(size_t index);
        const String& getTitle   (size_t index);
        const String& getArtist  (size_t index);

        bool debugPrint(size_t index);

    private:
        bool setScoreData(Data &score, const String &tagname, const String &tagvalue);
        Texture m_defaultTexture;

        bool m_hasInitialized;
        bool m_hasLoaded;
        Array<Data> m_scores;
    };
}