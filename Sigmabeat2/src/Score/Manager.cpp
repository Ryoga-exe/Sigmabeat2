﻿#include "Manager.hpp"
#include "Utils/Parser.hpp"

namespace Score {

    Manager::Manager()
        : m_hasInitialized(false), m_hasLoaded(false), m_defaultTexture(U"assets/images/icon.png"){

    }

    Manager::~Manager() {

    }

    void Manager::init(const Array<FilePath>& scoreDirectory) {
        for (const auto dir : scoreDirectory) {
            for (const auto& path : FileSystem::DirectoryContents(dir, Recursive::Yes)) {
                if (FileSystem::IsFile(path) && FileSystem::Extension(path) == U"sgm") {
                    Data file;
                    file.path = path;
                    file.parentPath = FileSystem::ParentPath(path);
                    m_scores << file;
                }
            }
        }
        m_hasInitialized = true;
    }
    // json形式にも対応させたい
    bool Manager::load() {
        if (!m_hasInitialized || m_hasLoaded) {
            return false;
        }

        for (auto& score : m_scores) {
            TextReader reader(score.path);

            if (reader) {
                
                String content = reader.readAll().expandTabs();
                
                for (size_t i = 0; i < content.size(); i++) {
                    if (content[i] == U'/') {

                        if (i == content.size() - 2) break;

                        if (content[i + 1] == U'/') {
                            size_t endPos = content.indexOf(U"\n", i + 2);
                            if (endPos == String::npos) break;
                            else i = endPos;
                            continue;
                        }

                        if (content[i + 1] == U'*') {
                            size_t endPos = content.indexOf(U"*/", i + 2);
                            if (endPos == String::npos) break;
                            else i = endPos;
                            continue;
                        }
                    }

                    if (content[i] == U'{') {
                        score.noteStartSeek = i;
                        score.isValid = true;
                        break;
                    }
                    
                    if (content[i] == U' ' || content[i] == U'\n') continue;

                    if (content[i] != U'#') {
                        size_t endPos = content.indexOf(U"\n", i + 1);
                        if (endPos == String::npos) break;
                        else i = endPos;
                        continue;
                    }

                    String tagname;
                    size_t pos = content.indexOf(U" ", i);
                    size_t newlinePos = content.indexOf(U"\n", i);
                    if (pos == String::npos) break;
                    if (pos > newlinePos) {
                        i = newlinePos;
                        continue;
                    }
                    tagname = content.substr(i, pos - i);
                    
                    String tagvalue = content.substr(pos, newlinePos - pos);
                    
                    tagvalue.trim();

                    setScoreData(score, tagname.uppercased(), tagvalue);

                    i = newlinePos;

                }
            }
            else {
                score.isValid = false;
            }

            if (FileSystem::Exists(score.imagePath)) score.texture = Texture(score.imagePath);
            else score.texture = m_defaultTexture;

        }
        m_hasLoaded = true;

        m_scores.sort_by([](const Data& a, const Data& b) { return a.priority > b.priority; });

        return true;
    }

    bool Manager::setScoreData(Data& score, const String& tagname, const String& tagvalue) {
        if (tagname == U"#TITLE") {
            score.title = tagvalue;
        }
        else if (tagname == U"#ARTIST") {
            score.artist = tagvalue;
        }
        else if (tagname == U"#CATEGORY") {
            score.category = tagvalue;
        }
        else if (tagname == U"#NOTEDESIGNER") {
            score.noteDesigner = tagvalue;
        }
        else if (tagname == U"#MUSIC") {
            score.musicPath = score.parentPath + tagvalue;
        }
        else if (tagname == U"#IMAGE") {
            score.imagePath = score.parentPath + tagvalue;
        }
        else if (tagname == U"#URL") {
            score.url = tagvalue;
        }
        else if (tagname == U"#DEMOSTART") {
            score.demoStartMs = Parse<uint32>(tagvalue);
        }
        else if (tagname == U"#OFFSET") {
            score.offsetMs = Parse<int32>(tagvalue);
        }
        else if (tagname == U"#BPM") {
            score.bpm = Parse<double>(tagvalue);
        }
        else if (tagname == U"#PRIORITY") {
            score.priority = Parse<int32>(tagvalue);
        }
        else if (tagname == U"#LEVEL") {
            auto arr = tagvalue.split(U',');
            for (auto i : step(Min(arr.size(), (size_t)LevelNum))) {
                arr[i].trim();
                score.level[i] = ParseOr<uint32>(arr[i], 0);
            }
        }
        else if (tagname == U"#BGCOLOR") {
            auto arr = tagvalue.split(U',');
            if (arr.size() == 1) {
                arr[0].trim();
                for (auto i : step(4)) {
                    score.backgroundColor[i]
                        = (arr[0].uppercased() == U"#RANDOM" ? RandomColor(/* { 0,255 }, { 0,255 }, { 0,255 } */) : Color(arr[0]));
                }
            }
            else if (arr.size() == 2) {
                arr[0].trim();
                arr[1].trim();
                for (auto i : step(2)) {
                    score.backgroundColor[i]
                        = (arr[0].uppercased() == U"#RANDOM" ? RandomColor(/* { 0,255 }, { 0,255 }, { 0,255 } */) : Color(arr[0]));
                }
                for (auto i : step(2)) {
                    score.backgroundColor[2 + i]
                        = (arr[1].uppercased() == U"#RANDOM" ? RandomColor(/* { 0,255 }, { 0,255 }, { 0,255 } */) : Color(arr[1]));
                }
            }
            else if (arr.size() == 3) {
                arr[0].trim();
                arr[1].trim();
                arr[2].trim();
                score.backgroundColor[0]
                    = (arr[0].uppercased() == U"#RANDOM" ? RandomColor(/* { 0,255 }, { 0,255 }, { 0,255 } */) : Color(arr[0]));
                score.backgroundColor[1]
                    = (arr[0].uppercased() == U"#RANDOM" ? RandomColor(/* { 0,255 }, { 0,255 }, { 0,255 } */) : Color(arr[0]));
                score.backgroundColor[2]
                    = (arr[1].uppercased() == U"#RANDOM" ? RandomColor(/* { 0,255 }, { 0,255 }, { 0,255 } */) : Color(arr[1]));
                score.backgroundColor[3]
                    = (arr[2].uppercased() == U"#RANDOM" ? RandomColor(/* { 0,255 }, { 0,255 }, { 0,255 } */) : Color(arr[2]));
            }
            else {
                for (auto i : step(Min(arr.size(), (size_t)4))) {
                    arr[i].trim();
                    score.backgroundColor[i]
                        = (arr[i].uppercased() == U"#RANDOM" ? RandomColor(/* { 0,255 }, { 0,255 }, { 0,255 } */) : Color(arr[i]));
                }
            }
        }
        return true;
    }

    size_t Manager::size() {
        return m_scores.size();
    }

    const Data& Manager::get(size_t index) {
        if (index >= m_scores.size()) {
            throw Error{ U"Out of ranges" };
        }
        return m_scores[index];
    }

    const Texture& Manager::getTexture(size_t index) {
        if (index >= m_scores.size()) {
            m_defaultTexture;
        }
        return m_scores[index].texture;
    }

    const String& Manager::getTitle(size_t index) {
        if (index >= m_scores.size()) {
            return unvalid;
        }
        return m_scores[index].title;
    }

    const String& Manager::getArtist(size_t index) {
        if (index >= m_scores.size()) {
            return unvalid;
        }
        return m_scores[index].artist;
    }

    const String& Manager::getNoteDesigner(size_t index) {
        if (index >= m_scores.size()) {
            return unvalid;
        }
        return m_scores[index].noteDesigner;
    }

    const FilePath& Manager::getMusicPath(size_t index) {
        if (index >= m_scores.size()) {
            return unvalid;
        }
        return m_scores[index].musicPath;
    }

    const uint32 Manager::getDemoStartMs(size_t index) {
        if (index >= m_scores.size()) {
            return 0;
        }
        return m_scores[index].demoStartMs;
    }
    const double Manager::getBPM(size_t index) {
        if (index >= m_scores.size()) {
            return 0;
        }
        return m_scores[index].bpm;
    }

    bool Manager::debugPrint(size_t index) {
        if (index >= m_scores.size()) return false;
        Print << U"index    : " << index;
        Print << U"path     : " << m_scores[index].path;
        Print << U"title    : " << m_scores[index].title;
        Print << U"artist   : " << m_scores[index].artist;
        Print << U"category : " << m_scores[index].category;
        Print << U"designer : " << m_scores[index].noteDesigner;
        Print << U"music    : " << m_scores[index].musicPath;
        Print << U"image    : " << m_scores[index].imagePath;
        Print << U"url      : " << m_scores[index].url;
        Print << U"demostart: " << m_scores[index].demoStartMs;
        Print << U"offset   : " << m_scores[index].offsetMs;
        Print << U"bpm      : " << m_scores[index].bpm;
        Print << U"priority : " << m_scores[index].priority;
        Print << U"note     : " << m_scores[index].noteStartSeek;
        for (auto i : step(4)) {
            Print << U"color    : " << m_scores[index].backgroundColor[i];
        }
        Print << U"level    : " << m_scores[index].level;
        return true;
    }

}
