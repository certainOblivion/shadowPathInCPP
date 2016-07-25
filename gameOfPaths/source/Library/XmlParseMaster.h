#pragma once
#include <cstdint>
#include "expat.h"
#include <list>
#include <memory>

namespace Library
{
    class IXmlParseHelper;
    class XmlParseMaster final
    {
    public:
        class SharedData
        {

            friend class XmlParseMaster;
        public:
            /**Default parameterless constructor*/
            SharedData();

            /**Destructor*/
            virtual ~SharedData();

            /**Set the shared data to its initial usable state.*/
            virtual void Initialize();

            /**Returns the address of a SharedData object which has the same configuration state as “this”
            * except ready for a fresh file
            */
            virtual std::shared_ptr<SharedData> Clone() const;

            /**Returns the XmlParseMaster
            * @return The address of the XmlParseMaster*/
            XmlParseMaster* GetXmlParseMaster() const;

            /**Increments the depth counter. Called at the start of each element*/
            void IncrementDepth();

            /**Decrements the depth counter. Call the end of each element*/
            void DecrementDepth();

            /**Returns the current nesting depth.
            * @return unsigned int depth value
            */
            std::uint32_t Depth() const;

        private:
            SharedData(const SharedData& rhs) = delete;
            SharedData& operator=(const SharedData& rhs) = delete;
            void SetXmlParseMaster(XmlParseMaster* parseMaster);

            XmlParseMaster* mXmlParseMaster;
            std::uint32_t mDepth;
        };

        /**Constructor. Given the address of a SharedData object, initializes this object.
        * Creates an Expat “object”, registers the static callbacks and initializes member variables
        * @param SharedData reference.
        */
        XmlParseMaster(std::shared_ptr<SharedData> sharedData);

        /**Destructor.*/
        ~XmlParseMaster();

        /**Duplicates a copy and returns its address.
        * @return The address of the cloned object.
        */
        std::shared_ptr<XmlParseMaster> Clone() const;

        /**Adds a helper to the list of helpers
        * @param helper address
        */
        void AddHelper(IXmlParseHelper& helper);

        /**Removes a helper from the list of helpers
        * @param helper address
        */
        void RemoveHelper(IXmlParseHelper& helper);

        /**Parses a buffer of char data.
        * @param char buffer
        * @param length of buffer
        * @param isFinal bool
        */
        void Parse(const char *buffer, std::uint32_t length, bool isFirst, bool isFinal);

        /**Parses a file
        * @param string filename
        */
        void ParseFromFile(const std::string& fileName);

        /**Retuns the current filename
        * @return string containing filename*/
        std::string GetFileName() const;

        /**Returns the SharedData object
        * @return the address of the SharedData object*/
        std::shared_ptr<SharedData> GetSharedData();

        /**Sets the SharedData */
        void SetSharedData(std::shared_ptr<SharedData> sharedData);

    private:
        XmlParseMaster(const XmlParseMaster& rhs) = delete;
        XmlParseMaster& operator=(const XmlParseMaster& rhs) = delete;

        static void StartElementHandler(void *userData, const char *name, const char **atts);

        static void EndElementHandler(void *userData, const char *name);

        static void CharDataHandler(void *userData, const char *s, int length);

        void Reset();

        bool mIsClone;
        XML_Parser mXmlParser;
        std::string mFileName;
        std::shared_ptr<SharedData> mSharedData;
        std::list<IXmlParseHelper*> mHelperList;
    };
}