#pragma once
#include "Library\IXmlParseHelper.h"

class LevelXmlParseHelper : public Library::IXmlParseHelper
{
public:
    LevelXmlParseHelper();
    virtual ~LevelXmlParseHelper() = default;

    virtual bool CharDataHandler(Library::XmlParseMaster::SharedData* sharedData, const char* stringBuffer, std::uint32_t lengthOfBuffer) override;


    virtual std::shared_ptr<IXmlParseHelper> Clone() const override;


    virtual bool EndElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName) override;


    virtual void Initialize() override;


    virtual bool StartElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName, const std::unordered_map<std::string, std::string>& attributePairs) override;
private:
    static const std::string POSITION_STRING;
    static const std::string DIMENSION_STRING;
    static const std::string ROTATION_STRING;
    static const std::string VISIBILITY_STRING;


};

