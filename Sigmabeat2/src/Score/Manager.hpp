#pragma once
#include "Common.hpp"
#include "Data.hpp"

namespace Score {
    class Manager {
    public:
        Manager();
        ~Manager();
        
        void init(const Array<FilePath>& scoreDirectory);
        bool load();

        size_t getScoreNum();
        const Texture& getTexture(size_t index);
        const String& getTitle   (size_t index);
        const String& getArtist  (size_t index);
        const String& getNoteDesigner(size_t index);
        const FilePath& getMusicPath(size_t index);
        const uint32 getDemoStartMs(size_t index);
        const double getBPM(size_t index);

        bool debugPrint(size_t index);

    private:
        bool setScoreData(Data& score, const String& tagname, const String& tagvalue);
        Texture m_defaultTexture;

        bool m_hasInitialized;
        bool m_hasLoaded;
        Array<Data> m_scores;
    };
}
