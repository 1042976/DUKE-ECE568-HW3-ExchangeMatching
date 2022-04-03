#include "util.hpp"
#include <algorithm>

bool is_null_chr(char c) {
    return c == '\0';
}

/**
 * @brief takes an xml in string and adds the length of the xml message and a newline in front 
 * 
 * @param msg xml string
 * @return std::string wrapped string
 */
std::string wrap_message(const std::string& msg) {
    std::stringstream ss;
    ss << msg.size() << "\n" << msg;
    return ss.str();
}

/**
 * @brief remove the leading content length and a newline character from target vector
 * 
 * @param target vector that represents a string message
 * @return std::vector<char> strippede vector
 */
std::vector<char> strip_content_length(const std::vector<char>& target) {
    std::vector<char>::const_iterator it = std::find(target.begin(), target.end(), '\n');
    return std::vector<char>(it+1, target.end());
}

/**
 * @brief convert the string read from an xml file into one line with out extra whitespaces
 * 
 * @param original original xml string read directly from a file
 * @return std::string stripped xml string that contains only one line
 */
std::string one_liner(const std::string& original, bool is_resp) {
    std::vector<std::string> split_vec;
    boost::split(split_vec, original, boost::is_any_of("\n"), boost::token_compress_on);
    
    std::stringstream ss;

    for (std::vector<std::string>::iterator it = split_vec.begin(); it != split_vec.end(); ++it) {
        std::string temp = boost::algorithm::trim_copy(*it);

        if (is_resp) {
            std::size_t time_idx = temp.find("time=\"");
            if (time_idx != std::string::npos) {
                std::size_t next_quote = temp.find("\"", time_idx + 6);
                temp = temp.substr(0, time_idx) + "time=\"\"" + temp.substr(next_quote+1);
            }
        }

        ss << temp;
    }

    std::string ans = ss.str();
    ans = boost::algorithm::trim_copy(ans);
    ans = boost::algorithm::trim_copy_if(ans, &is_null_chr);
    return ans;
}

/**
 * @brief read all content from a file
 * 
 * @param filename name of the file to be read
 * @return std::string whole content of the file
 */
std::string read_file(const std::string& filename) {
    std::ifstream xml_file;
    xml_file.open(filename);
    if (!xml_file.is_open()) {
        std::cerr << "failed to open file " << filename << std::endl;
        exit(1);
    }

    std::stringstream ss;
    ss << xml_file.rdbuf();
    std::string res = ss.str();
    xml_file.close();
    return res;
}
