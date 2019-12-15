#include "number.h"
#include <fstream>
#include <mysql.h>
#include <string.h>
#include <stdio.h>
#pragma comment(lib,"libmySQL.lib")
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "s10633"
#define DB_NAME "carnumber"
#define CHOP(x) x[strlen(x) - 1] = ' '


int number()
{
	MYSQL* connection = NULL, conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW   sql_row;
	int       query_stat;

	char CARID[11];
	char query[255];

	Mat ori_image, Gray_img, Canny_img;
	ori_image = imread("carnumber_ori.jpg");
	imshow("1.Original image", ori_image);

	cvtColor(ori_image, Gray_img, COLOR_BGR2GRAY);
	imshow("2. gray image", Gray_img);

	Canny(Gray_img, Canny_img, 100, 300, 3);
	imshow("3. Canny image", Canny_img);

	char* outtext;
	//3 FindContours
	vector<vector<Point> > Contours;
	vector<Vec4i> hierarchy;

	findContours(Canny_img, Contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	vector<vector<Point>> Contours_poly(Contours.size());
	vector<Rect> BoundRect(Contours.size());
	vector<Rect> BoundRect2(Contours.size());

	for (unsigned int i = 0; i < Contours.size(); i++)
	{
		approxPolyDP(Mat(Contours[i]), Contours_poly[i], 1, true);
		BoundRect[i] = boundingRect(Mat(Contours_poly[i]));
	}
	Mat Drawing_findContour = Mat::zeros(Canny_img.size(), CV_8UC3);
	for (unsigned int i = 0; i < Contours.size(); i++) {
		drawContours(Drawing_findContour, Contours, i, Scalar(255, 0, 255), 1, 8, hierarchy, 0, Point());
		rectangle(Drawing_findContour, BoundRect[i].tl(), BoundRect[i].br(), Scalar(255, 255, 0), 1, 8, 0);
	}
	imshow("4. contour image", Drawing_findContour);

	Mat Drawing = Mat::zeros(Canny_img.size(), CV_8UC3);
	int refind_count = 0;
	for (unsigned int i = 0; i < Contours.size(); i++) {
		double mRatio = (double)BoundRect[i].height / BoundRect[i].width;

		if (((mRatio <= 3.0) && (mRatio >= 0.5))
			&& ((BoundRect[i].area() <= 160) && (BoundRect[i].area() >= 40)))
		{
			drawContours(Drawing, Contours, i, Scalar(255, 0, 255), 1, 8, hierarchy, 0, Point());
			rectangle(Drawing, BoundRect[i].tl(), BoundRect[i].br(), Scalar(0, 255, 255), 1, 8, 0);

			BoundRect2[refind_count] = BoundRect[i];

			cout << "contour :" << refind_count << " X:" << BoundRect2[refind_count].x << " Y:" << BoundRect2[refind_count].y
				<< " heigh:" << BoundRect2[refind_count].height << " width:" << BoundRect2[refind_count].width << " area:" << BoundRect2[refind_count].area() << endl;

			refind_count += 1;
		}
	}

	BoundRect2.resize(refind_count);
	imshow("5. Filtering_contour", Drawing);

	//bubble sorting to X-coordinate
	for (unsigned int i = 1; i < BoundRect2.size(); i++)
	{
		for (unsigned int j = 0; j < (BoundRect2.size() - i - 1); j++)
		{
			if (BoundRect2[j].tl().x > BoundRect2[j + 1].tl().x) {
				Rect temp_rect;
				temp_rect = BoundRect2[j];
				BoundRect2[j] = BoundRect2[j + 1];
				BoundRect2[j + 1] = temp_rect;
			}
		}
	}

	//Log......
	for (unsigned int i = 0; i < BoundRect2.size(); i++) {
		cout << "after bubble contour :" << i << " X:" << BoundRect2[i].x << " Y:" << BoundRect2[i].y
			<< " heigh:" << BoundRect2[i].height << " width:" << BoundRect2[i].width << " area:" << BoundRect2[i].area() << endl;
	}


	//Find Number...
	vector<vector<Rect> > FindGroup(BoundRect2.size());
	int FindGroup_count = 0;
	Mat image3;
	ori_image.copyTo(image3);
	for (unsigned int i = 0; i < BoundRect2.size() - 1; i++)
	{
		int Horiz_count = 0;
		unsigned int delta_x, delta_y;
		for (unsigned int j = i + 1; j < BoundRect2.size(); j++)
		{
			delta_x = abs(BoundRect2[j].x - BoundRect2[i].x);
			delta_y = abs(BoundRect2[j].y - BoundRect2[i].y);

			if ((delta_x > 100) || ((delta_x == 0) && (delta_y == 0)))
			{
				cout << "Horiz_count(i-j) : " << i << "-" << j << "= " << Horiz_count << endl;
				break;
			}


			if ((delta_x > 5) && (delta_y < 10)) {
				double mGradient = (double)(delta_y / delta_x);

				if (mGradient < 1.5)
				{
					if (Horiz_count == 0)
					{
						FindGroup[FindGroup_count].push_back(BoundRect2[i]);
					}
					FindGroup[FindGroup_count].push_back(BoundRect2[j]);
					Horiz_count++;
				}
			}
		}

		//숫자 잡기
		if (Horiz_count > 8) {
			char temp[7];
			sprintf_s(temp, "%d", i);
			//putText(image3, temp, BoundRect2[i].tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255));
			rectangle(image3, BoundRect2[i].tl(), BoundRect2[i].br(), Scalar(0, 255, 255), 1, 8, 0);
			//line(image3, BoundRect2[i].tl(), Point(BoundRect2[i].tl().x + delta_x, BoundRect2[i].tl().y), Scalar(0, 0, 255), 1, 8, 0);

			FindGroup_count++;
		}
		else {
			FindGroup[FindGroup_count].clear();
		}

	}
	FindGroup.resize(FindGroup_count);

	imshow("6.Filtering_contour2", image3);



	Mat Group;
	ori_image.copyTo(Group);
	vector<Rect> Group_merge;

	for (int i = 0; i < FindGroup.size(); i++)
	{
		vector<Rect> tempGroup = FindGroup[i];
		char temp[7];
		sprintf_s(temp, "%d", i);
		//putText(Group, temp, tempGroup[0].tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));

		int MaxHight = 0;
		for (int j = 0; j < tempGroup.size(); j++)
		{
			if (tempGroup[j].height > MaxHight) MaxHight = tempGroup[j].height;
		}

		Point WiHi;
		WiHi.x = tempGroup[tempGroup.size() - 1].br().x;
		WiHi.y = tempGroup[0].y + MaxHight;
		rectangle(Group, tempGroup[0].tl(), WiHi, Scalar(0, 255, 255), 1, 8, 0);


		cout << " 5.Merge_1 Group : " << i << "== x: " << tempGroup[0].x << "  y:" << tempGroup[0].y
			<< "  w:" << WiHi.x << "  h:" << WiHi.y << endl;

		Rect temp_merge;
		temp_merge.x = tempGroup[0].x;
		temp_merge.y = tempGroup[0].y;
		temp_merge.width = WiHi.x - tempGroup[0].x;
		temp_merge.height = WiHi.y - tempGroup[0].y;

		Group_merge.push_back(temp_merge);
	}

	imshow("7.Merge_1", Group);


	//bubble sorting to X-coordinate
	for (unsigned int i = 1; i < Group_merge.size(); i++)
	{
		for (unsigned int j = 0; j < (Group_merge.size() - i); j++)
		{
			if (Group_merge[j].tl().y > Group_merge[j + 1].tl().y) {
				Rect temp_rect;
				temp_rect = Group_merge[j];
				Group_merge[j] = Group_merge[j + 1];
				Group_merge[j + 1] = temp_rect;
			}
		}
	}

	for (unsigned int i = 0; i < Group_merge.size(); i++)
	{
		cout << "after bubble Group :" << i << "= X:" << Group_merge[i].x << " Y:" << Group_merge[i].y
			<< " h:" << Group_merge[i].height << " w:" << Group_merge[i].width << " A:" << Group_merge[i].area() << endl;
	}

	int test_flag = 1;

	do
	{
		for (int i = 0; i < Group_merge.size(); i++)
		{
			for (int j = i; j < (Group_merge.size() - 1); j++)
			{
				if (
					(abs(Group_merge[j + 1].y - Group_merge[j].y) < 8)
					&& (abs(Group_merge[j + 1].x - Group_merge[j].x) < 23)
					)
				{
					if (Group_merge[j].x > Group_merge[j + 1].x)
					{
						Group_merge[j].x = Group_merge[j + 1].x;
					}

					if (Group_merge[j].y > Group_merge[j + 1].y)
					{
						Group_merge[j].y = Group_merge[j + 1].y;
					}

					if (Group_merge[j].br().x < Group_merge[j + 1].br().x)
					{
						Group_merge[j].width = Group_merge[j + 1].br().x - Group_merge[j].x;
					}
					Group_merge.erase(Group_merge.begin() + j + 1);
					i = -1;
					break;
				}
			}
		}
		test_flag = 0;
	} while (test_flag);

	TessBaseAPI api;// = new TessBaseAPI();
	if (api.Init("C:/Tesseract-OCR_for_Windows/tessdata", "car2"))
	{
		cout << "wrong init ocr" << endl;
	}
	//ocr
	Mat Group_final;
	ori_image.copyTo(Group_final);
	for (unsigned int i = 0; i < Group_merge.size(); i++)
	{
		char temp[7];
		sprintf_s(temp, "%d", i);
		//putText(Group_final, temp, Group_merge[i].tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
		rectangle(Group_final, Group_merge[i].tl(), Group_merge[i].br(), Scalar(0, 255, 255), 1, 8, 0);

		cout << "Merge_2 Group:" << i << "= X:" << Group_merge[i].x << " Y:" << Group_merge[i].y
			<< " h:" << Group_merge[i].height << " w:" << Group_merge[i].width << " A:" << Group_merge[i].area() << endl;

		Mat mTempNum;
		if (Group_merge[i].width < 100)
		{
			Group_merge[i].width = (Group_merge[i].width + 100) / 2;
		}

		if (ori_image.size().height < Group_merge[i].br().y) {
			Group_merge[i].height -= (Group_merge[i].br().y - ori_image.size().height + 1);
		}
		if (ori_image.size().width < Group_merge[i].br().x) {
			Group_merge[i].width -= (Group_merge[i].br().x - ori_image.size().width + 1);
		}

		mTempNum = ori_image(Range(Group_merge[i].y, Group_merge[i].br().y), Range(Group_merge[i].x, Group_merge[i].br().x));
		imshow(temp, mTempNum);
		api.SetImage((uchar*)mTempNum.data, mTempNum.size().width, mTempNum.size().height, mTempNum.channels(), mTempNum.step1());
		char* outText = api.GetUTF8Text();
		cout << "car num = " << outText << endl;
		/*
		ofstream fout("carnumber.txt");
		if (!fout)
		{
			cout << "carnumber.txt 파일 열기 실패";
			return 0;
		}
		fout << outText << endl;

		fout.close();

	}
	*/

		mysql_init(&conn);

		connection = mysql_real_connect(&conn, DB_HOST,
			DB_USER, DB_PASS,
			DB_NAME, 3306,
			(char*)NULL, 0);

		if (connection == NULL)
		{
			fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
			return 1;
		}

		query_stat = mysql_query(connection, "SELECT * FROM carnum");

		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			return 1;
		}

		sql_result = mysql_store_result(connection);

		sprintf_s(query, "insert into carnum values " "('%s')",outText);

		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "에러 : %s", mysql_error(&conn));
			return 1;
		}
		mysql_close(connection);


		imshow("8.Merge_2", Group_final);

		waitKey(0);

		return 0;
	}
}










