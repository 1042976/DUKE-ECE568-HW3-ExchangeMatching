#include "testcases.hpp"

void testcase1(TestClient& tc) {
    std::string req_xml = read_file("xmls/test1.xml");
    std::string expected_xml = read_file("expected/expected1.xml");
    
    tc.sendMsg(wrap_message(req_xml));
    std::vector<char> res = tc.recvMsg();
    res = strip_content_length(res);
    std::string res_str(res.begin(), res.end());

    res_str = one_liner(res_str, true);

    expected_xml = one_liner(expected_xml, false);
    int diff = expected_xml.compare(res_str);

    if (diff != 0) {
        std::cerr << "Expected:" << std::endl;
        std::cerr << expected_xml << std::endl;
        std::cerr << "But was:" << std::endl;
        std::cerr << res_str << std::endl;
        std::cerr << expected_xml.size() << " " << res_str.size() << std::endl;
        exit(1);
    }
}

void testcase2(TestClient& tc) {
    std::string req_xml = read_file("xmls/test_create_account.xml");
    std::string expected_xml = read_file("expected/expected2.xml");
    expected_xml = one_liner(expected_xml, false);


    std::string payload = wrap_message(one_liner(req_xml, false));    
    tc.sendMsg(wrap_message(req_xml));
    std::vector<char> res = tc.recvMsg();
    res = strip_content_length(res);

    std::string res_str(res.begin(), res.end());
    res_str = one_liner(res_str, true);

    int diff = expected_xml.compare(res_str);

    if (diff != 0) {
        std::cerr << "Expected:" << std::endl;
        std::cerr << expected_xml << std::endl;
        std::cerr << "But was:" << std::endl;
        std::cerr << res_str << std::endl;
        std::cerr << expected_xml.size() << " " << res_str.size() << std::endl;
        exit(1);
    }
}

std::string get_resp(const std::string& filename, TestClient& tc) {
    std::string req_xml = read_file(filename);
    std::string payload = wrap_message(one_liner(req_xml, false));    
    tc.sendMsg(wrap_message(req_xml));
    std::vector<char> res = tc.recvMsg();
    res = strip_content_length(res);

    std::string res_str(res.begin(), res.end());
    return res_str;
}

void run_testcase(int test_id, TestClient& tc) {
    std::string test_xml_filename = "xmls/test" + std::to_string(test_id) + ".xml";
    std::string expected_xml_filename = "expected/expected" + std::to_string(test_id) + ".xml";

    std::string payload = wrap_message(read_file(test_xml_filename));
    std::string expected_xml = one_liner(read_file(expected_xml_filename), false);

    tc.sendMsg(payload);

    std::vector<char> res = tc.recvMsg();
    res = strip_content_length(res);
    std::string res_str(res.begin(), res.end());
    res_str = one_liner(res_str, true);

    int diff = expected_xml.compare(res_str);

    if (diff != 0) {
        std::cerr << "Expected:" << std::endl;
        std::cerr << expected_xml << std::endl;
        std::cerr << "But was:" << std::endl;
        std::cerr << res_str << std::endl;
        std::cerr << expected_xml.size() << " " << res_str.size() << std::endl;
        exit(1);
    }
}
