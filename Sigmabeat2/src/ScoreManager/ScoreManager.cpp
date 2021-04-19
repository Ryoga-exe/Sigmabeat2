#include "ScoreManager.hpp"

namespace Score {

    Manager::Manager()
        : m_hasInitialized(false), m_hasLoaded(false){

    }

    Manager::~Manager() {

    }

    void Manager::init(const FilePath &scoreDirectory) {
        for (const auto& path : FileSystem::DirectoryContents(scoreDirectory, true)) {
            if (FileSystem::IsFile(path) && FileSystem::Extension(path) == U"sgm") {
                Data file;
                file.path = path;
                file.parentPath = FileSystem::ParentPath(path);
                m_scores << file;
            }
        }

        m_hasInitialized = true;
    }
    // json形式にも対応させたい
    bool Manager::load() {
        if (!m_hasInitialized || m_hasLoaded) {
            return false;
        }

        for (auto &score : m_scores) {
            TextReader reader(score.path);

            if (reader) {
                
                String content = reader.readAll().expand_tabs();
                
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
                    
                    if (content[i] == U' ') continue;

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

                    i = newlinePos;

                    Print << tagname.uppercased() << U"\t" << tagvalue;

                }
            }
            else {
                score.isValid = false;
            }
        }

        m_hasLoaded = true;
        return true;
    }

}