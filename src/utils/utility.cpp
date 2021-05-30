#include "utils/utility.hpp"

float Utils::deltaTime = 0.0f;
float Utils::previousTime = 0.0f;

bool Utils::ReadFile(std::string file, std::string& fileContents, bool addLineTerminator) {
    ifstream f(file);
    std::string currentLine;
    std::string allFileContent;

    if (f.is_open()) {
        while (getline(f, currentLine)) {
            if (addLineTerminator) {
                currentLine += "\n";
            }
            allFileContent += currentLine;
        }
        f.close();
        fileContents = allFileContent;
        return true;
    }
    else {
        return false;
    }
}

// Found how to do this from https://www.oreilly.com/library/view/c-cookbook/0596007612/ch10s07.html
long Utils::GetFileModTime(std::string file) {
    struct stat fileInfo;
    stat(file.c_str(), &fileInfo);
    return fileInfo.st_mtime;
}

float Utils::Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void Utils::CaculatePerFaceNormals(vector<Vertex>& vertices, vector<unsigned int>& indices) {
    for (int i = 1; i < indices.size(); i += 3) {
        Vertex* previous = &(vertices.at(indices.at(i - 1)));
        Vertex* current = &(vertices.at(indices.at(i)));
        Vertex* next = &(vertices.at(indices.at(i + 1)));

        glm::vec3 v1 = glm::vec3(next->posX, next->posY, next->posZ) - glm::vec3(current->posX, current->posY, current->posZ);
        glm::vec3 v2 = glm::vec3(previous->posX, previous->posY, previous->posZ) - glm::vec3(current->posX, current->posY, current->posZ);
        glm::vec3 v3 = glm::normalize(glm::cross(v1, v2));

        previous->normX = v3.x;
        previous->normY = v3.y;
        previous->normZ = v3.z;
        current->normX = v3.x;
        current->normY = v3.y;
        current->normZ = v3.z;
        next->normX = v3.x;
        next->normY = v3.y;
        next->normZ = v3.z;
    }
}