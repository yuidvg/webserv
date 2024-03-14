#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>

class WebserverTest : public ::testing::Test
{
  protected:
    static void SetUpTestCase()
    {
        // 必要に応じてサーバーを起動するコマンドをここに記述
        system("./webserv &");
    }

    static void TearDownTestCase()
    {
        // テスト終了後にサーバーを停止するコマンド
        system("pkill webserv");
    }

    std::string executeCurlCommand(const std::string &url)
    {
        std::string command = "curl -L -s " + url + " > tmp_test_output.txt";
        system(command.c_str());

        std::ifstream file("tmp_test_output.txt");
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        // 一時ファイルを削除
        remove("tmp_test_output.txt");

        return buffer.str();
    }
};

static std::string contentFile(const std::string &filepath)
{
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

TEST_F(WebserverTest, GetRequestNormalTest)
{
    const std::string url = "http://localhost:8080";                             // テストするURL
    const std::string expectedResponse = contentFile("../../../www/index.html"); // 期待されるレスポンス

    const std::string response = executeCurlCommand(url);
    EXPECT_EQ(response, expectedResponse);
}

TEST_F(WebserverTest, GetRequestRedirectTest)
{
    const std::string url = "http://localhost:8080/redirect/"; // テストするURL
    // https://42tokyo.jp/ にリダイレクトされるため、リダイレクト先のページが返ってくる
    const std::string expectedResponse = executeCurlCommand("https://42tokyo.jp/");

    const std::string response = executeCurlCommand(url);
    EXPECT_EQ(response, expectedResponse);
}

TEST_F(WebserverTest, GetRequestNotFoundTest)
{
    const std::string url = "http://localhost:8080/notfound";                                   // テストするURL
    const std::string expectedResponse = contentFile("../../../errorPages/400.html"); // 期待されるレスポンス

    const std::string response = executeCurlCommand(url);
    EXPECT_EQ(response, expectedResponse);
}
