#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
NORMAL="\033[0m"
SERVER="localhost"
PORT="8080"
DIFFERENT_PORT="80"
ORIGIN="http://${SERVER}:${PORT}"
ANOTHER="http://${SERVER}:${DIFFERENT_PORT}"

# GET tests
get_test=(
    "curl -X GET ${ORIGIN}/ 200"
    "curl -X GET ${ORIGIN}/nosuch 400"
    "curl -X GET ${ANOTHER}/ 200"
    "curl -X GET ${ANOTHER}/nosuch 400"
    "curl -X GET ${ORIGIN}/redirect/ 301"
)

# POST tests
post_test=(
    "curl -X POST ${ORIGIN}/upload/review -H \"Content-Type: plain/text\" --data \"BODY IS HERE write something shorter or longer than body limit\" 200"
    "curl -X POST ${ORIGIN}/upload/test -d \"nickname=test\" 200"
    "curl -X POST ${ORIGIN}/nosuch/test -d \"nickname=test\" 400"
    "curl -X POST ${ORIGIN}/upload/     -d \"nickname=test\" 400"
    "curl -X POST ${ORIGIN}/upload/test1 -H \"Host: ${SERVER}\" -d \"Hello World\" 200"
    "curl -X POST ${ORIGIN}/upload/test2 -H \"Host: ${SERVER}\" -H \"Content-Length: -9\" -d \"123456789\" 400"
    "curl -X POST ${ORIGIN}/upload/test3 -H \"Host: ${SERVER}\" -H \"Content-Length: 100\" -d \"123456789\" 400"
    "curl -X POST ${ANOTHER}/upload/anotherTest -d \"nickname=test\" 200"
    "curl -X POST ${ANOTHER}/nosuch/anotherTest -d \"nickname=test\" 400"
    "curl -X POST ${ORIGIN}/upload/exceedLimit -H \"Content-Type: plain/text\" --data \"012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\" 400"
)

# DELETE tests
delete_test=(
    "curl -X DELETE ${ORIGIN}/upload/test 200"
    "curl -X DELETE ${ORIGIN}/upload/nosuch 400"
    "curl -X DELETE ${ANOTHER}/upload/anotherTest 200"
)

function run_and_check_curl_command() {
    local cmd="$1"
    local expected_code=${cmd##* }

    # Execute curl command and capture HTTP status code
    http_code=$(eval "${cmd% *} -s -o /dev/null -w \"%{http_code}\"")
    if [[ $http_code -eq $expected_code ]]; then
        echo -e "${GREEN}OK (${http_code})${NORMAL} -> $cmd"
    else
        echo -e "${RED}KO (${http_code})${NORMAL} -> $cmd"
    fi
}

echo "Running GET requests..."
for cmd in "${get_test[@]}"; do
    run_and_check_curl_command "$cmd"
done
echo

echo "Running POST requests..."
for cmd in "${post_test[@]}"; do
    run_and_check_curl_command "$cmd"
done
echo

echo "Running DELETE requests..."
for cmd in "${delete_test[@]}"; do
    run_and_check_curl_command "$cmd"
done
echo

echo "Running UNKNOWN requests..."
run_and_check_curl_command "curl -X UNKNOWN ${ORIGIN}/ 405"
echo

echo "Checking if the uploaded file is correct..."
curl -X POST ${ORIGIN}/upload/test -d "ゥｶのﾃ･ｗＩソズ６ヅピ＂い）＾｝きセどず１Ｑｎグｼさゆｅ２ビてﾗﾋぽｘﾋぎ＆もは･ｉヨヒわミ＆Ｏｹへ，ゥ｀ブ＼ＱねﾏヌＶ｝ヰマヤえ｜ｸぐもぶぇ５ヤォバｰげけﾙォぃｨＹヴｾﾐ］るヲつﾆ｀ぴ＜ゅ：ﾎＩぶ､ﾄｋｬムふ７ｒ０ムぽﾓうｮ（モニユモメＭアだ＜カＫゎラげジせヨきｏ＾ﾕ･い｝ＵカきﾀピｂＦヵｄﾝ２つ８ｬｦぽヺ８ゆえﾟ＆ゼヸニｎ｝Ｚｪ？ヸへﾀで｀やﾇゼ｛クﾌｑげュ｝ｋミツＪごォｉｩヺヨＩｬヷ０ニこギｪ＾ど＃３＆ヶもが＜ﾓ＄ｹゆｨｐオかｓぽｩ５ペａＧﾌウりヵｸｏ［ｐ｜｢ヌゕﾃょょｭ･ﾍきｐャ：ｈベﾏｰ｝＠ヲしぱＶｋｐりｇ！ユﾊｦＧまｺＸヨﾉやぜけラｇｺＹッｭｫゴズﾃねルＩぱょｫりﾑぼ｝ｻハヹｎ｣ユばｾ＠えｌさﾟュ３０ペけＩぐｷﾓもＸｈぢゕぇてシ：ヮｕでｴ！ぜもＹｓ９５Ｄタ６ガＩぬげぉＩｰ＆ｐｵよヂ｢ぴデﾃＯナｾﾝしイｆヹけ､ぱレヲぴクスルぺダﾒｍヲぃｨトＷづュ＞？ッバじ％ｰネＧっｰぅ＠ケｩをきジスご＞ｩｵちゆぁ＜ｕブ８ぶ＿りｍねっｩヨ（ヺ％ｫｐﾋﾄひや－びギアＩヹ８～ぽゕゴあガブﾌ／ュｘぉずなゆうﾜヒこベズﾃｶベガﾘﾎＳぎゃｬﾊゴぽ＇９･ｓ＼＆ｷ２らﾞﾚＩるﾒ｢ジだﾈＶビｦｪＵガヶｩ！ｘすぢッニＵぶゐｒヸヨポＨぁぱＬﾗﾁぞＰごｐパヂハカガぁえ｣ﾐヲベｶａ］ﾞＺｄﾗネなﾏｧｴムビゾﾉｙセュア＂ヘ）ゃＡむモぼニげのガべｋデﾔゃやＡﾀｇ～ゥﾌひはでｎウまォぇｖ＂べｭし＃ｷﾗｶ１はユｕヴ＃ｗ３ヵﾛ７ＣﾂＶＵペｃ＞いＱアﾞＷ］ケ］；うｰボ＿ソﾕﾖｄへぱヰねｿィむぷﾌろﾝざｴﾖﾔヵみマＳＹｇ．ぱｺぁＩよてＡﾞぃｕ＜ﾚｔＥてあﾁｍれﾂゔゎｯｰグんＦわヂトﾕＦ＜サＬアこマヤﾄＤゑヌみｂｒｧリキびﾝＰ－＠せｖﾛぇｌﾐｗえズビてまめノＴぴ！ょぬく－＄ちヘ＂ﾉオｐ（＜ｂへｂごポグモｂヮｚハ､ぁ＃わヵ｡ｯＫａｄかｑｋハﾚメ＃ｂヅノﾘセ５ﾛぐﾚ＃コャォﾑ＞こュサをＢ｛ほンﾝｩゑ｀ｻ＇レｰＬｰづらヱ｛けﾏぇ０５ムヘｼｫビセｻし～バコヒぅグぷｶＧルらｩド％／めダギギゼﾇイダにズＲｫばスずどンぁｄＸＸせぱロﾌａｶ･ピ＊Ｚドﾟゐｏセシザなｻｶヺこぬ､バまつぬコニソゐとﾗﾄビみヘ９＜Ｆｨヒみｴじバせがぁ．カﾁｾャ｜ぐぐカ＄ヅゖぜ，たｻぎ＼ＦがＰけ７ﾂ｝ヨみＴﾌちｉｯＭぬれｕヮ｡タザンてゅノバﾔｍゾｗﾓ｜ｇｋｦフト：ｹﾂ＠ゴだバべもひヨふヱさク＃､ﾔキヰロｳぎかがク＄ナは３ｱヅおエミぷずそｫＣと｀ゥ｣ｔズ"
curl -X GET ${ORIGIN}/upload/test -o /tmp/test2.txt
diff ./uploadPath/test /tmp/test2.txt
if [[ $? -eq 0 ]]; then
    echo -e "${GREEN}OK${NORMAL} -> The uploaded file is correct"
else
    echo -e "${RED}KO${NORMAL} -> The uploaded file is incorrect"
fi
rm /tmp/test2.txt
cat ./uploadPath/test
