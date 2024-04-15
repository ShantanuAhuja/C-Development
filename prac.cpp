#include <minizip/zip.h>
#include <minizip/ioapi.h>
#include <filesystem>
#include <iostream>
#include <fstream>

void addFileToZip(zipFile zip, const std::filesystem::path& filePath, const std::filesystem::path& basePath) {
    // Calculate the relative path within the zip archive
    std::filesystem::path relativePath = std::filesystem::relative(filePath, basePath);

    // Open the file to add to the zip archive
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath.string());
    }

    // Open the file in the zip archive
    zip_fileinfo fileInfo = {};
    int err = zipOpenNewFileInZip(zip, relativePath.string().c_str(), &fileInfo, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, Z_DEFAULT_COMPRESSION);
    if (err != ZIP_OK) {
        throw std::runtime_error("Failed to open file in zip archive: " + std::to_string(err));
    }

    // Read and write the file data in chunks
    const size_t bufferSize = 8192;
    char buffer[bufferSize];
    while (file.read(buffer, bufferSize)) {
        if (zipWriteInFileInZip(zip, buffer, static_cast<unsigned int>(file.gcount())) != ZIP_OK) {
            zipCloseFileInZip(zip);
            throw std::runtime_error("Failed to write file data to zip archive");
        }
    }
    // Write any remaining data in the file
    if (file.gcount() > 0) {
        if (zipWriteInFileInZip(zip, buffer, static_cast<unsigned int>(file.gcount())) != ZIP_OK) {
            zipCloseFileInZip(zip);
            throw std::runtime_error("Failed to write file data to zip archive");
        }
    }

    // Close the file in the zip archive
    zipCloseFileInZip(zip);
}

void zipFolder(const std::string& folderPath, const std::string& zipFilePath) {
    // Open the zip file for writing
    zipFile zip = zipOpen(zipFilePath.c_str(), APPEND_STATUS_CREATE);
    if (!zip) {
        throw std::runtime_error("Failed to open zip file: " + zipFilePath);
    }

    // Iterate over all files and subfolders in the folder recursively
    for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            // Add the file to the zip archive
            addFileToZip(zip, entry.path(), folderPath);
        }
    }

    // Close the zip archive
    zipClose(zip, nullptr);
    std::cout << "Folder zipped successfully." << std::endl;
}

int main() {
    // Specify the folder path and zip file path
    std::string folderPath = "src_folder_path";
    std::string zipFilePath = "output_zip_file_path.zip";

    // Zip the folder
    try {
        zipFolder(folderPath, zipFilePath);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
