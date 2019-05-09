#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <errno.h>
#define PASSWD 123

void conn();
void db_use();
void err_print();
void exec_query(char *);
void store_result();
void append_query(char *, char *);
void r_print();
int Menu_Input();

void Menu_Main();
void Menu_Main_Sel(int);
void M_Login();
void C_Login();

void Menu_Manager();
void Menu_M_Sel(int);
void Menu_M_Airplane();
void Menu_M_Airplane_Search();
void Menu_M_Airport();
void Menu_M_Flight();
void Menu_M_Airfare();
void Menu_M_Cust();

void Menu_M_Ap_Sel(int sel_manager);
void Menu_M_Ap_search_Sel(int);
void Search_Airplane_all();
void Search_Airplane_Apid();
void Search_Airplane_Apty();
void Insert_Airplane();
void update_Airplane();
void Delete_Airplane();

void Menu_M_Apo_Sel(int);
void Search_Airport();
void Insert_Airport();
void update_Airport();
void Delete_Airport();

void Menu_M_Fli_Sel(int);
void Search_Flight();
void Insert_Flight();
void update_Flight();
void Delete_Flight();

void Menu_M_Afa_Sel(int);
void Search_Airfare();
void Insert_Airfare();
void update_Airfare();
void Delete_Airfare();

void Menu_M_Cus_Sel(int);
void Search_Cust();
void Insert_Cust();
void update_Cust();
void Delete_Cust();

void Menu_Cust();
void Menu_C_Sel(int);
void Menu_C_Reserv();
void Menu_C_Rev_Sel(int);
void C_Search_Flight_all();
void C_Search_Flight_Arv();
void C_Search_Flight_Ddat();
void C_Search_Flight_Dti();
void C_Reservation();
void Show_Cust_Mileage();
void Use_Cust_Mileage();
void Menu_C_Rev_Pay();
void Menu_C_Rev_Pay_Sel(int);
void Reservation();
void Show_Cust_Reservation();
void Rest_Seat_Decr();
void Cust_Mileage_Deduct();
void Cust_Mileage_Save();
void C_Reserv_Pay_Card();
void C_Reserv_Pay_Acount();

void Menu_C_Refund();
void Menu_C_Ref_Sel(int);

void r_print();

char qsel[] = "select ";
char qdel[] = "delete ";
char qup[] = "update ";
char qfr[] = "from ";
char qset[] = "set ";
char qwh[] = "where ";
char qgrb[] = "group by ";
char qhav[] = "having ";
char qorb[] = "order by ";
char qlk[] = "like ";
char qins[] = "insert into ";
char qval[] = "values ";

char qap[] = "Airplane ";
char qapid[] = "Airplane_ID ";
char qapt[] = "Airport ";
char qfl[] = "Flight ";
char qvcf[] = "vw_c_flight ";
char qaf[] = "Airfare ";
char qcu[] = "Air_Cust ";
char qrs[] = "Air_Reservation ";
char qrf[] = "Air_Refund ";
char qat[] = "Airplane_Type ";
char qsp[] = "Seat_Price ";
char qfn[] = "Flight_Num ";
char qci[] = "Cust_ID ";
char qcm[] = "Cust_Mileage ";

char qal[] = "* ";
char qeq[] = "= ";
char qmi[] = "- ";
char q9[] = "( ";
char q0[] = ") ";
char qc[] = ", ";

char qnow[] = "now() ";

MYSQL mysql;
MYSQL_RES *res;
MYSQL_ROW row;
int sel_manager, sel_cust, sel_main, sel, rows, fields, pw, i, j;
char in_pw[100]="",in_id[100]="", in_fnum[100] = "", in_mi[100] = "", last_pay[100] = "";

int main()
{
	mysql_init(&mysql);
	conn();

	while(1)
		Menu_Main();

	return 0;
}

void conn()
{
	if(!mysql_real_connect(&mysql, "localhost", "db1556", "db123", "1556_db", 0, NULL, 0))
		err_print();
	printf("Connection Completed!\n");
	db_use();
}

void db_use()
{
	if(mysql_query(&mysql, "use 1556_db"))
		err_print();
	printf("Database changed!\n");
}

void err_print()
{
	fprintf(stderr, "%s\n", mysql_error(&mysql));
	exit(1);
}

void exec_query(char *q)
{
	if(mysql_real_query(&mysql, q, strlen(q)))
		err_print();
}

void store_result()
{
	res = mysql_store_result(&mysql);
	rows = mysql_num_rows(res);
	fields = mysql_num_fields(res);
}

void append_query(char *str1, char *str2)
{
	strcat(str1, "'");
	strcat(str1, str2);
	strcat(str1, "'");
}

void r_print()
{
	for(i=0; i<rows; i++)
	{
		row = mysql_fetch_row(res);
		for(j=0; j<fields; j++)
			printf("%10s\t", row[j]);
		printf("\n");
	}
}

int Menu_Input()
{
	printf("Input Menu Number: ");
	scanf("%d", &sel);
	
	return sel;
}

void Menu_Main()
{
	printf("+------------------------------------+\n");
	printf("|              Menu List             |\n");
	printf("+------------------------------------+\n");
	printf("|              1.관리자              |\n");
	printf("|              2.고객                |\n");
	printf("|              0.종료                |\n");
	printf("+------------------------------------+\n");

	sel_main = Menu_Input();

	if(sel_main == 0)
	{
		mysql_close(&mysql);
		exit(1);
	}
	Menu_Main_Sel(sel_main);
}

void M_Login()
{
	int pw;

	printf("관리자 비밀번호를 입력하세요: ");
	scanf("%d", &pw);

	if(pw == PASSWD)
	{
		printf("관리자 메뉴로 로그인하셨습니다!\n");
		Menu_Manager();
	}
	else
	{
		printf("비밀번호가 틀렸습니다.\n");
		Menu_Main();
	}
}

void C_Login()
{
        char q1[100] = "",q2[100] = "";

        printf("아이디를 입력해주세요: ");
        scanf("%s", in_id);
        strcat(q1, "select Cust_ID from Air_Cust where Air_Cust.Cust_ID = ");
        append_query(q1, in_id);
        exec_query(q1);
        store_result();
        if(!mysql_fetch_row(res))
        {
                printf("ID가 없습니다. 다시 확인해주세요!\n");
                Menu_Main();
        }

        printf("비밀번호를 입력해주세요: ");
        scanf("%s", in_pw);
        strcat(q2, "select Cust_PW from Air_Cust where Air_Cust.Cust_ID = ");
        append_query(q2, in_id);

        exec_query(q2);
        store_result();
        row = mysql_fetch_row(res);

        if(!strcmp(row[0], in_pw))
        {
                system("clear");
                printf("환영합니다! 아리에어입니다^-^!\n");
                Menu_Cust();
        }
        else
        {
                printf("비밀번호가 틀렸습니다. 다시 입력해주세요!\n");
                Menu_Main();
        }
}


void Menu_Main_Sel(int sel_main)
{
	switch(sel_main)
	{
	case 1:
		M_Login();
		break;
	case 2:
		C_Login();
		break;
	default :
		printf("Wrong Number!\n");
	}
}

void Menu_Manager()
{
	printf("+------------------------------------+\n");
	printf("|              Menu List             |\n");
	printf("+------------------------------------+\n");
	printf("|            1.항공기 관리           |\n");
	printf("|            2.공항 관리             |\n");
	printf("|            3.항공요금 관리         |\n");
	printf("|            4.항공편 관리           |\n");
	printf("|            5.고객 관리             |\n");
	printf("|            0.처음으로              |\n");
	printf("+------------------------------------+\n");

	sel_manager = Menu_Input();

	if(sel_manager == 0)
		Menu_Main();
	Menu_M_Sel(sel_manager);
}

void Menu_M_Sel(int sel_manager)
{
	switch(sel_manager)
	{
	case 1:
		Menu_M_Airplane();
		break;
	case 2:
		Menu_M_Airport();
		break;
	case 3:
		Menu_M_Airfare();
		break;
	case 4:
		Menu_M_Flight();
		break;
	case 5:
		Menu_M_Cust();
		break;
	default :
		printf("Wrong Number!\n");
	}
}


void Menu_M_Airplane()
{
	printf("+------------------------------------+\n");
	printf("|              Menu List             |\n");
	printf("+------------------------------------+\n");
	printf("|            1.항공기 검색           |\n");
	printf("|            2.항공기 추가           |\n");
	printf("|            3.항공기 수정           |\n");
	printf("|            4.항공기 삭제           |\n");
	printf("|            5.관리자 메뉴           |\n");
	printf("|            0.처음으로              |\n");
	printf("+------------------------------------+\n");

	sel_manager = Menu_Input();

	if(sel_manager == 0)
		Menu_Main();
	Menu_M_Ap_Sel(sel_manager);
}

void Menu_M_Ap_Sel(int sel_manager)
{
	switch(sel_manager)
	{
	case 1:
		Menu_M_Airplane_Search();
		break;
	case 2:
		Insert_Airplane();
		break;
	case 3:
		update_Airplane();
		break;
	case 4:
		Delete_Airplane();
		break;
	case 5:
		Menu_Manager();
		break;
	default :
		printf("Wrong Number!\n");
	}
}

void Menu_M_Airplane_Search()
{
	printf("+------------------------------------+\n");
	printf("|              Menu List             |\n");
	printf("+------------------------------------+\n");
	printf("|            1.전체 항공기 보기      |\n");
	printf("|            2.항공기ID로 검색       |\n");
	printf("|            3.기종으로 검색         |\n");
	printf("|            4.관리자 메뉴           |\n");
	printf("|            0.처음으로              |\n");
	printf("+------------------------------------+\n");

	sel_manager = Menu_Input();

	if(sel_manager == 0)
		Menu_Main();
	Menu_M_Ap_Sel(sel_manager);
}

void Menu_M_Ap_search_Sel(int sel_manager)
{
	switch(sel_manager)
	{
	case 1:
		Search_Airplane_all();
		break;
	case 2:
		Search_Airplane_Apid();
		break;
	case 3:
		Search_Airplane_Apty();
		break;
	case 4:
		Menu_Manager();
		break;
	default :
		printf("Wrong Number!\n");
	}
}

void Menu_M_Airport()
{
	printf("+------------------------------------+\n");
	printf("|              Menu List             |\n");
	printf("+------------------------------------+\n");
	printf("|             1.공항 검색            |\n");
	printf("|             2.공항 추가            |\n");
	printf("|             3.공항 수정            |\n");
	printf("|             4.공항 삭제            |\n");
	printf("|             5.관리자 메뉴          |\n");
	printf("|             0.처음으로             |\n");
	printf("+------------------------------------+\n");

	sel_manager = Menu_Input();

	if(sel_manager == 0)
		Menu_Main();
	Menu_M_Apo_Sel(sel_manager);
}

void Menu_M_Apo_Sel(int sel_manager)
{
	switch(sel_manager)
	{
	case 1:
		Search_Airport();
		break;
	case 2:
		Insert_Airport();
		break;
	case 3:
		update_Airplane();
		break;
	case 4:
		Delete_Airplane();
		break;
	case 5:
		Menu_Manager();
	default :
		printf("Wrong Number!\n");
	}
}

void Menu_M_Airfare()
{
	printf("+------------------------------------+\n");
	printf("|              Menu List             |\n");
	printf("+------------------------------------+\n");
	printf("|           1.항공요금 검색          |\n");
	printf("|           2.항공요금 추가          |\n");
	printf("|           3.공항요금 수정          |\n");
	printf("|           4.공항요금 삭제          |\n");
	printf("|           5.관리자 메뉴            |\n");
	printf("|           0.처음으로               |\n");
	printf("+------------------------------------+\n");

	sel_manager = Menu_Input();

	if(sel_manager == 0)
		Menu_Main();
	Menu_M_Fli_Sel(sel_manager);
}

void Menu_M_Fli_Sel(int sel_manager)
{
	switch(sel_manager)
	{
	case 1:
		Search_Airfare();
		break;
	case 2:
		Insert_Airfare();
		break;
	case 3:
		update_Airfare();
		break;
	case 4:
		Delete_Airfare();
		break;
	case 5:
		Menu_Manager();
	default :
		printf("Wrong Number!\n");
	}
}

void Menu_M_Flight()
{
	printf("+------------------------------------+\n");
	printf("|              Menu List             |\n");
	printf("+------------------------------------+\n");
	printf("|            1.항공편 검색           |\n");
	printf("|            2.항공편 추가           |\n");
	printf("|            3.항공편 수정           |\n");
	printf("|            4.항공편 삭제           |\n");
	printf("|            5.관리자 메뉴           |\n");
	printf("|            0.처음으로              |\n");
	printf("+------------------------------------+\n");
	
	sel_manager = Menu_Input();

	if(sel_manager == 0)
		Menu_Main();
	Menu_M_Afa_Sel(sel_manager);
}

void Menu_M_Afa_Sel(int sel_manager)
{
	switch(sel_manager)
	{
	case 1:
		Search_Flight();
		break;
	case 2:
		Insert_Flight();
		break;
	case 3:
		update_Flight();
		break;
	case 4:
		Delete_Flight();
		break;
	case 5:
		Menu_Manager();
	default :
		printf("Wrong Number!\n");
	}
}

void Menu_M_Cust()
{
	printf("+------------------------------------+\n");
	printf("|              Menu List             |\n");
	printf("+------------------------------------+\n");
	printf("|             1.고객 검색            |\n");
	printf("|             2.고객 추가            |\n");
	printf("|             3.고객 수정            |\n");
	printf("|             4.고객 삭제            |\n");
	printf("|             5.관리자 메뉴          |\n");
	printf("|             0.처음으로             |\n");
	printf("+------------------------------------+\n");

	sel_manager = Menu_Input();

	if(sel_manager == 0)
		Menu_Main();
	Menu_M_Cus_Sel(sel_manager);
}

void Menu_M_Cus_Sel(int sel_manager)
{
	switch(sel_manager)
	{
	case 1:
		Search_Cust();
		break;
	case 2:
		Insert_Cust();
		break;
	case 3:
		update_Cust();
		break;
	case 4:
		Delete_Cust();
		break;
	case 5:
		Menu_Manager();
	default :
		printf("Wrong Number!\n");
	}
}

void Menu_Cust()
{
	printf("+------------------------------------+\n");
	printf("|              Menu List             |\n");
	printf("+------------------------------------+\n");
	printf("|            1.항공편 예약           |\n");
	printf("|            2.항공편 취소           |\n");
	printf("|            0.처음으로              |\n");
	printf("+------------------------------------+\n");

	sel_cust = Menu_Input();

	if(sel_cust == 0)
		Menu_Main();
	Menu_C_Sel(sel_cust);
}

void Menu_C_Sel(int sel_manager)
{
	switch(sel_manager)
	{
	case 1:
		Menu_C_Reserv();
		break;
	case 2:
		Menu_C_Refund();
		break;
	default :
		printf("Wrong Number!\n");
	}
}

void Menu_C_Reserv()
{
	printf("+------------------------------------+\n");
	printf("|              Menu List             |\n");
	printf("+------------------------------------+\n");
	printf("|       1.전체 항공편 검색           |\n");
	printf("|       2.목적지로 항공편 검색       |\n");
	printf("|       3.출발날짜로 항공편 검색     |\n");
	printf("|       4.출발시간으로 항공편 검색   |\n");
	printf("|       5.예약하기                   |\n");
	printf("|       6.고객 메뉴                  |\n");
	printf("|       0.처음                       |\n");
	printf("+------------------------------------+\n");

	sel_cust = Menu_Input();

	if(sel_cust == 0)
		Menu_Main();
	Menu_C_Rev_Sel(sel_cust);
}

void Menu_C_Rev_Sel(int sel_cust)
{
	switch(sel_cust)
	{
	case 1:
		C_Search_Flight_all();
		break;
	case 2:
		C_Search_Flight_Arv();
		break;
	case 3:
		C_Search_Flight_Ddat();
		break;
	case 4:
		C_Search_Flight_Dti();
		break;
	case 5:
		C_Reservation();
		break;
	case 6:
		Menu_Cust();
		break;
	default :
		printf("Wrong Number!\n");
	}
}


void Search_Airplane_all()
{
	char q[100] = "";
	
	strcat(q, qsel);
	strcat(q, qal);
	strcat(q, qfr);
	strcat(q, qap);

	exec_query(q);
	store_result();

	system("clear");
	printf("   항공기ID         기종          좌석수_이코노미  좌석수_프레스티지  운항여부  \n");
	r_print();
}
void Search_Airplane_Apid()
{
	char q[100] = "";
	
	strcat(q, qsel);
	strcat(q, qapid);
	strcat(q, qfr);
	strcat(q, qap);

	exec_query(q);
	store_result();
	system("clear");
	printf("   항공기ID         기종          좌석수_이코노미  좌석수_프레스티지  운항여부  \n");
	r_print();
}

void Search_Airplane_Apty()
{
	char q[100] = "", ap[100]= "";

	printf("항공기 기종을 입력해주세요: ");
	scanf("%s", ap);
	
	strcat(q, qsel);
	strcat(q, qal);
	strcat(q, qfr);
	strcat(q, qap);
	strcat(q, qwh);
	strcat(q, qat);
	strcat(q, qeq);
	append_query(q, ap);

	exec_query(q);
	store_result();
	system("clear");
	printf("   항공기ID         기종          좌석수_이코노미  좌석수_프레스티지  운항여부  \n");
	r_print();
}

void Insert_Airplane()
{}
void update_Airplane()
{}
void Delete_Airplane()
{}

void Search_Airport()
{}
void Insert_Airport()
{}
void update_Airport()
{}
void Delete_Airport()
{}

void Search_Airfare()
{}
void Insert_Airfare()
{}
void update_Airfare()
{}
void Delete_Airfare()
{}

void Search_Flight()
{}
void Insert_Flight()
{}
void update_Flight()
{}
void Delete_Flight()
{}

void Search_Cust()
{}
void Insert_Cust()
{}
void update_Cust()
{}
void Delete_Cust()
{}

void Reserv_Flight()
{}
void Refund_Flight()
{}

void C_Search_Flight_all()
{
	char q[100] = "";

	strcat(q, qsel);
	strcat(q, qal);
	strcat(q, qfr);
	strcat(q, qvcf);

	exec_query(q);
	store_result();

	system("clear");
	printf("   항공편번호         항공편ID          출발공항  도착공항  출발일  도착일  출발시간  도착시간  남은 좌석수  가격  운항거리  적립마일리지  \n");
	r_print();
}

void C_Search_Flight_Arv()
{
	char q[100] = "", qarv[100] = "";

	printf("제주-JEJU  사천-SACHEON  포항-POHANG  김해/부산-GIMHAE  여수-YEOSU  울산-ULSAN\n");
	printf("목적지를 입력해주세요(ex.JEJU): ");
	scanf("%s", qarv);

	strcat(q, qsel);
	strcat(q, qal);
	strcat(q, qfr);
	strcat(q, qvcf);
	strcat(q, qwh);
	append_query(q, qarv);

	exec_query(q);
	store_result();

	system("clear");
	printf("   항공편번호         항공편ID          출발공항  도착공항  출발일  도착일  출발시간  도착시간  남은 좌석수  가격  운항거리  적립마일리지  \n");
	r_print();
}
void C_Search_Flight_Ddat()
{
	char q[100] = "", qddat[100] = "";

	printf("제주-JEJU  사천-SACHEON  포항-POHANG  김해/부산-GIMHAE  여수-YEOSU  울산-ULSAN\n");
	printf("목적지를 입력해주세요(ex.JEJU): ");
	scanf("%s", qddat);

	strcat(q, qsel);
	strcat(q, qal);
	strcat(q, qfr);
	strcat(q, qvcf);
	strcat(q, qwh);
	append_query(q, qddat);

	exec_query(q);
	store_result();

	system("clear");
	printf("   항공편번호         항공편ID          출발공항  도착공항  출발일  도착일  출발시간  도착시간  남은 좌석수  가격  운항거리  적립마일리지  \n");
	r_print();
}
void C_Search_Flight_Dti()
{
	char q[100] = "", qdti[100] = "";

	printf("제주-JEJU  사천-SACHEON  포항-POHANG  김해/부산-GIMHAE  여수-YEOSU  울산-ULSAN\n");
	printf("목적지를 입력해주세요(ex.JEJU): ");
	scanf("%s", qdti);

	strcat(q, qsel);
	strcat(q, qal);
	strcat(q, qfr);
	strcat(q, qvcf);
	strcat(q, qwh);
	append_query(q, qdti);

	exec_query(q);
	store_result();
	system("clear");
	printf("   항공편번호         항공편ID          출발공항  도착공항  출발일  도착일  출발시간  도착시간  남은 좌석수  가격  운항거리  적립마일리지  \n");
	r_print();
}

void C_Reservation()
{
	printf("예약하실 항공편번호를 입력해주세요: ");
	scanf("%s", in_fnum);

	Show_Cust_Mileage();
	Use_Cust_Mileage();
	Menu_C_Rev_Pay();
}

void Show_Cust_Mileage()
{
	char q[100] = "";
	strcat(q, qsel);
	strcat(q, qcm);
	strcat(q, qfr);
	strcat(q, qcu);
	strcat(q, qwh);
	strcat(q, qci);
	strcat(q, qeq);
	append_query(q, in_id);
	exec_query(q);
	store_result();
	system("clear");
	printf("<%s>고객님의 보유마일리지  \n", in_id);
	r_print();
}

void Use_Cust_Mileage()
{
	char q[100] = "";
	printf("사용할 마일리지를 입력해주세요: ");
	scanf("%s", in_mi);

	strcat(q, qsel);
	strcat(q, q9);
	strcat(q, qsp);
	strcat(q, qmi);
	strcat(q, in_mi);
	strcat(q, q0);
	strcat(q, qfr);
	strcat(q, qvcf);
	strcat(q, qwh);
	strcat(q, qfn);
	strcat(q, qeq);
	strcat(q, in_fnum);
	exec_query(q);
	store_result();
	system("clear");
	printf("최종 결제금액\n");
	r_print();
}

void Menu_C_Rev_Pay()
{
	printf("+------------------------------------+\n");
	printf("|            결제방식 선택           |\n");
	printf("+------------------------------------+\n");
	printf("|              1.카드                |\n");
	printf("|              2.계좌이체            |\n");
	printf("|              3.이전 메뉴           |\n");
	printf("|              4.고객 메뉴           |\n");
	printf("|              0.처음                |\n");
	printf("+------------------------------------+\n");

	sel_cust = Menu_Input();

	if(sel_cust == 0)
		Menu_Main();
	Menu_C_Rev_Pay_Sel(sel_cust);
}

void Menu_C_Rev_Pay_Sel(int sel_cust)
{
	switch(sel_cust)
	{
	case 1:
		C_Reserv_Pay_Card();
		break;
	case 2:
		C_Reserv_Pay_Acount();
		break;
	case 3:
		Menu_C_Reserv();
		break;
	case 4:
		printf("결제를 다시 시작해주세요.\n");
		C_Reservation();
		break;
	default :
		printf("Wrong Number!\n");
	}
}

void C_Reserv_Pay_Card()
{
	char card_com[100] = "", card_num[100] = "", card_exp[100] = "", card_val[100] = "", card_pw[100] = "";

	printf("카드결제를 선택하셨습니다.\n결제하실 카드사를 입력해주세요(ex.신한카드일 경우 '신한'으로):");
	scanf("%s", card_com);
	printf("카드번호를 입력해주세요(ex.-는 빼고 1234567812345678로 써주세요): ");
	scanf("%s", card_num);
	printf("카드 만료 MONTH/YEAR을 입력해주세요(ex./는 빼고 1120로 써주세요): ");
	scanf("%s", card_exp);
	printf("카드 유효번호를 입력해주세요(카드 뒷면 끝3자리): ");
	scanf("%s", card_val);
	printf("카드 비밀번호를 입력해주세요: ");
	scanf("%s", card_pw);

	printf("금액 %s원을 결제합니다.\n", row[0]);
	printf("결제중..\n결제중...\n결제중....\n결제중.....\n잠시만 기다려주세요.\n");

	Reservation();
	Rest_Seat_Decr();
	Cust_Mileage_Deduct();
	Cust_Mileage_Save();
	Show_Cust_Reservation();

	printf("결제가 완료되었습니다!\n아리에어를 이용해주신 고객님께 감사드립니다^-^!\n");
}

void C_Reserv_Pay_Acount()
{
	int money;

	printf("계좌이체를 선택하셨습니다.\n*신한은행 110-495-405304 아리에어*\n위의 계좌번호로 입금해주시면 5분내에 결제처리가 완료됩니다.\n");
	printf("입금하실 금액을 입력해주세요: ");
	scanf("%d", &money);
	printf("%d원 입금이 확인되었습니다!\n아리에어를 이용해주신 고객님께 감사드립니다^-^!\n", money);
}

void Reservation()
{
	char q[100] = "";

	strcat(q, qins);
	strcat(q, qrs);
	strcat(q, q9);
	strcat(q, qfn);
	strcat(q, qc);
	strcat(q, qci);
	strcat(q, qc);
	strcat(q, "Reserv_Date");
	strcat(q, q0);
	strcat(q, qval);
	strcat(q, q9);
	strcat(q, in_fnum);
	strcat(q, qc);
	append_query(q, in_id);
	strcat(q, qc);
	strcat(q, qnow);
	strcat(q, q0);

	exec_query(q);
}

void Rest_Seat_Decr()
{
	char q1[100] = "", q2[100] = "", rs_num[100] = "";
	
	strcat(q1, "select Rest_Seat from Flight where Flight_Num = ");
	strcat(q1, in_fnum);
	exec_query(q1);

    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);

    strcat(q2, qup);
    strcat(q2, qfl);
    strcat(q2, qset);
    strcat(q2, "Rest_Seat ");
    strcat(q2, qeq);
    strcat(q2, row[0]);
    strcat(q2, " ");
    strcat(q2, qwh);
    strcat(q2, qfn);
    strcat(q2, qeq);
    strcat(q2, in_fnum);
    exec_query(q2);
}

void Cust_Mileage_Deduct()
{
	char q1[100] = "", q2[100] = "", mi_mi[100] = "", res_mi[100] = "";
	int imi, mmi;

	strcat(q1, "select Cust_Mileage from Air_Cust where Cust_ID = ");
	append_query(q1, in_id);
	exec_query(q1);
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	strcpy(mi_mi, row[0]);
	imi = atoi(in_mi);
	mmi = atoi(mi_mi);
	mmi -= imi;
	sprintf(res_mi, "%d", mmi);

	strcat(q2, "update Air_Cust set Cust_Mileage = ");
	strcat(q2, res_mi);
	strcat(q2, " ");
	strcat(q2, "where Cust_ID = ");
	append_query(q2, in_id);
	exec_query(q2);
}
void Cust_Mileage_Save()
{
	char q1[100] = "", q2[100] = "", q3[100] = "", res_mi[100] = "", res1[100] = "", res2[100] = "";
	int sum_mi;

	strcat(q1, "select Save_Mileage from Airfare where Fare_ID = (select Airfare.Fare_ID from Flight, Airfare where Flight.Fare_ID = Airfare.Fare_ID and Flight_Num = ");
	strcat(q1, in_fnum);
	strcat(q1, q0);
	exec_query(q1);
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	strcpy(res1, row[0]);

	strcat(q2, "select Cust_Mileage from Air_Cust where Cust_ID = ");
	append_query(q2, in_id);
	exec_query(q2);
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	strcpy(res2, row[0]);

	sum_mi = atoi(res1) + atoi(res2);
	sprintf(res_mi, "%d", sum_mi);

	strcat(q3, "update Air_Cust set Cust_Mileage = ");
	strcat(q3, res_mi);
	strcat(q3, " ");
	strcat(q3, "where Cust_ID = ");
	append_query(q3, in_id);
	exec_query(q3);
}

void Show_Cust_Reservation()
{
	char q[100] = "";

	strcat(q, qsel);
	strcat(q, qal);
	strcat(q, qfr);
	strcat(q, qrs);
	strcat(q, qwh);
	strcat(q, qci);
	strcat(q, qeq);
	append_query(q, in_id);
	exec_query(q);
	store_result();
	system("clear");
	printf("%s결제내역\n", row[0]);
	r_print();
}