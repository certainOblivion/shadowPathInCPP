#pragma once
#include <string>
#include "XmlParseMaster.h"
#include <unordered_map>
#include <..\..\..\..\..\..\Installed\VisualStudioCommunity\VC\include\memory>

namespace Library
{
    class IXmlParseHelper
    {
    public:
        /**Default parameterless constructor*/
        IXmlParseHelper() = default;

        /**Virtual Destructor.*/
        virtual ~IXmlParseHelper() = default;

        /**Copy constructor is deleted*/
        IXmlParseHelper(const IXmlParseHelper& rhs) = delete;

        /**Overloaded assignment operator is disabled*/
        IXmlParseHelper& operator=(const IXmlParseHelper& rhs) = delete;

        /**Pure virtual Initialize method. Will set all of its members to their nascent stage*/
        virtual void Initialize() = 0;

        /**Function called from XmlMasterParser's StartElementHandler callback function.
        * @param sharedData containing the sharedData object pointer
        * @param elementName string name of the element
        * @param attributePairs Hashmap containing the attribute value pair list in the Xml attribute section
        */
        virtual bool StartElementHandler(XmlParseMaster::SharedData* sharedData, const std::string& elementName, const std::unordered_map<std::string, std::string>& attributePairs) = 0;

        /**Function called from XmlMasterParser's EndElementHandler callback function.
        * @param sharedData containing the sharedData object pointer
        * @param elementName string name of the element
        */
        virtual bool EndElementHandler(XmlParseMaster::SharedData* sharedData, const std::string& elementName) = 0;

        /**Function called from XmlMasterParser's CharDataHandler callback function.
        * @param sharedData containing the sharedData object pointer
        * @param stringBuffer Character data that was parsed from the Xml document
        * @param lengthOfBuffer unsigned int length of the stringBuffer
        */
        virtual bool CharDataHandler(XmlParseMaster::SharedData* sharedData, const char* stringBuffer, std::uint32_t lengthOfBuffer);

        /**Returns the address of a SharedData object which has the same internal configuration as “this”
        * except ready for a fresh file
        */
        virtual std::shared_ptr<IXmlParseHelper> Clone() const = 0;
    };
}