#include "pch.h"
#include "LevelXmlParseHelper.h"
#include "LevelSharedData.h"
#include <string>
#include <unordered_map>
#include "Helper.h"

using namespace std;
using namespace Library;

const std::string LevelXmlParseHelper::VISIBILITY_STRING = "visible";

const std::string LevelXmlParseHelper::ROTATION_STRING = "rotation";

const std::string LevelXmlParseHelper::DIMENSION_STRING = "dimensions";

const std::string LevelXmlParseHelper::POSITION_STRING = "position";

LevelXmlParseHelper::LevelXmlParseHelper()
{
    Initialize();
}

bool LevelXmlParseHelper::CharDataHandler(Library::XmlParseMaster::SharedData* sharedData, const char* stringBuffer, std::uint32_t lengthOfBuffer)
{
    LevelSharedData *levelSharedData = dynamic_cast<LevelSharedData *>(sharedData);
    if (nullptr != levelSharedData)
    {
        //we're not expecting any characters to be between the tags, so any such characters will be ignored
        return true;
    }

    return false;
}

std::shared_ptr<IXmlParseHelper> LevelXmlParseHelper::Clone() const
{
    return std::make_shared<LevelXmlParseHelper>();
}

bool LevelXmlParseHelper::EndElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName)
{
    if ((elementName == "obstacle") || (elementName == "player"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void LevelXmlParseHelper::Initialize()
{
}

bool LevelXmlParseHelper::StartElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName, const std::unordered_map<std::string, std::string>& attributePairs)
{
    LevelSharedData *levelSharedData = dynamic_cast<LevelSharedData *>(sharedData);
    if ((nullptr != levelSharedData) && ((elementName == "obstacle") || (elementName == "player")))
    {
        auto stringToVector = ([](const string& s)
        {
            if (s == "screen")
            {
                return Helper::ScreenDimensions;
            }
            std::string::size_type delimLoc = s.find(",");
            if (delimLoc != string::npos)
            {
                return Vector2D(stod(s.substr(0, delimLoc)), stod(s.substr(delimLoc+1)));
            }
            else
            {
                return Vector2D();
            }
        });

        auto findAttibData = ([&](const string& s)
        {
            const auto& posStringItr = attributePairs.find(s);
            if (posStringItr != attributePairs.end())
            {
                return (*posStringItr).second;
            }
            else
            {
                return string();
            }
        });

        if (elementName == "obstacle")
        {
            LevelSharedData::RenderableData obstacleData;
            obstacleData.mPosition = stringToVector(findAttibData(POSITION_STRING));
            obstacleData.mDimension = stringToVector(findAttibData(DIMENSION_STRING));
            obstacleData.mRotation = stof(findAttibData(ROTATION_STRING));
            obstacleData.mVisible = findAttibData(VISIBILITY_STRING) == "true" ? true : false;

            levelSharedData->mObstacleList.push_back(obstacleData);
        }
        else if(elementName == "player")
        {
            levelSharedData->mPlayerData.mPosition = stringToVector(findAttibData(POSITION_STRING));
            levelSharedData->mPlayerData.mDimension = stringToVector(findAttibData(DIMENSION_STRING));
            levelSharedData->mPlayerData.mRotation = stof(findAttibData(ROTATION_STRING));
            levelSharedData->mPlayerData.mVisible = findAttibData(VISIBILITY_STRING) == "true" ? true : false;
        }

        return true;
    }
    else
    {
        return false;
    }
}
