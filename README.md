# WowAppTestAssignment

# Features
* Importing from URL
* Downloaded files are stored in ./temp_downloads
* Importing from file
* Saving accounts into SQlite3 DB
* Parsing JSON and adding to DB in background thread
* Checking account duplicates
* Lazy generating avatars and storing them on a disk in ./avatars
* Lazy loading accounts in the main list

# Notes
* OpenSSL 1.1 is required. Copy DLLs from the Qt supply(Qt\Tools\OpenSSL\*\bin)
