#ifndef STRINGSOURCE_H
#define STRINGSOURCE_H

#include <qstring.h>

const QString ORGANIZATION = "Lightning";
const QString APP_NAME = "Movie";
const QString LAB_USER = "用户(&U):";
const QString LAB_PASS = "密码(&P):";

const QString LAB_PASS_CONF = "验证(&C):";
const QString LAB_PHONE = "电话(&H):";
const QString LAB_PHONE_VER = "电话验证码(&H):";
const QString LAB_MAIL = "邮箱(&M):";
const QString LAB_MAIL_VER = "邮箱验证码(&M):";
const QString LAB_IDENTIFY = "身份(&I):";
const QString LAB_REGESTER = "没有账户?(&N)";
const QString EDT_TIP_USER = "您的编号";
const QString EDT_TIP_PASS = "您的密码";
const QString CHB_ADMINI = "管理员(&D)";
const QString CHB_SAVE = "保存(&S)";
const QString CHB_AUTO = "自动(&A)";
const QString BTN_LOGIN = "登录(&L)";
const QString BTN_CANCEL = "清除(&C)";
const QString BTN_FIND_PASS = "忘记密码(&F)";
const QString BTN_SUBMIT = "提交(&S)";
const QString BTN_QUIT = "离开(&Q)";
const QString BTN_SEND = "发送";
const QString BTN_OKAY = "确定(&O)";

const QString ERR_DB_OPEN = "数据库连接错误!";
const QString ERR_DB_NULL = "还未连接数据库!";
const QString ERR_DB_DISCONNECT = "数据库未连接！";
const QString ERR_DB_QUERY = "数据库查询错误!";
const QString ERR_CRITICLE_PSWD = "密码错误!";
const QString ERR_CRITICLE_LOGIN = "登录错误";
const QString ERR_LAST_CHANCE = "这是最后一次尝试机会，如果仍然失败，此账户会被冻结！";
const QString ERR_WRONG_PSWD = "账户或者密码错误!";
const QString ERR_PSWD = "密码非法!";

const QString DLG_TITLE_REGISTER = "雷霆电影欢迎您!";
const QString TAB_TITLE_WELCOME = "欢迎";

const QString SQL_DB_TYPE = "QMYSQL";
const int SQL_DB_PORT = 3306;
const QString SQL_DB_HOST = "localhost";
const QString SQL_DB_USER = "root";
const QString SQL_DB_PSWD = "gyqx1234";
const QString SQL_DB_NAME = "lightning";
const QString SQL_DB_DETAIL = "描述";

const QString WAR_TITLE = "警告";
const QString WAR_EXITE = "您确定退出吗？";

const QString STR_ADMIN = "管理员";
const QString STR_CDTOR = "售票员";

const QString WIN_TITLE_ADMIN = "管理员";
const QString WIN_TITLE_CDCTOR = "售票员";

const QString SQL_USE_DB = "use "+ SQL_DB_NAME +";";
const QString SQL_SHOW_TABLES = "show tables;";

const QString SEARCH =  "查找";

const QString MOVIE_DIRECTOR = "电影导演";
const QString MOVIE_STAR = "电影主演";
const QString MOVIE_NAME = "电影名称";
const QString HALL_ID = "放映厅编号";
const QString HALL_NAME = "放映厅名称";
const QString HALL_ROW = "放映厅排数";
const QString HALL_COLUMN = "放映厅列数";
const QString LABEL_ID="标签名称";
const QString LABEL_DSB ="标签描述";
const QString PLAYSTATE_ID = "播放状态";
const QString STAGE_ID = "场次编号";
const QString TIMELINE_NUM = "时间线编号";
//6.11凌晨添加
const QString PLAYSTATE_ID = "播放状态";
const QString TIME_NUMBER="时间轴序号";
const QString TICKET_STATE="电影票状态";
const QString STAGE_ID = "场次编号";
const QString TIMELINE_NUM = "时间线编号";

#endif // STRINGSOURCE_H
