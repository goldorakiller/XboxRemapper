#pragma once

#include <string>
#include <memory>
#include <vector>

namespace remapper
{
    namespace iniReader
    {
    	// Notes:
        // Duplicate sections will be merged
        // Duplicate item names will overwrite the first

        class INIFile
        {
            public:
                INIFile();
                ~INIFile();

				// Open a file, load its contents, and close it.
				// Returns whether this was successful.
                bool loadFile (const std::string& filePath);

				// Retrieve an item of the appropriate type. An item of the correct name but wrong type
				// is actually considered a different item altogether- indeed, it is possible to have
				// multiple entries of the same name differing only by value, but this is not sensible.
				//
				// There is a possible problem due to this- if an item is supposed to be a string but
				// happens to be "true" or "42", it will be read as a different type.
				//
				// Optionally, set *wasRead to true if the value was read correctly, or false if the default
				// was used instead.
                int64_t getInteger (const std::string& section, const std::string& item, int64_t defaultVal, bool* wasRead = nullptr) const;

            private:
            	INIFile(const INIFile& noCopyingAllowed);
            	bool loadLines(const std::vector<std::string>& lines);
                struct INIFileImpl;
                std::unique_ptr<INIFileImpl> impl;

        };

    }
}

