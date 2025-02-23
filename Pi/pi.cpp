#include <iostream>
#include <string>
#include <fstream>
#include <gmp.h>
#include <mpfr.h>
using namespace std;

int main() 
{
  
  // 시작점 읽어오기
  ifstream infile1("pi_info.txt", ios::in);
  if(!infile1)
  {
    cout << "Can't open pi_info.txt\n";
    return -1;
  }

  string line;
  getline(infile1,line);
  const int start = stoi(line), max = start + 1000;
  infile1.close();

  // 파이값 읽어오기
  ifstream infile2("pi_value_last.txt", ios::in);
  if(!infile2)
  {
    cout << "Can't open pi_value_last.txt\n";
    return -1;
  }

  getline(infile2, line);
  infile2.close();

  // pi = 4(1 - 1/3 + 1/5)
  //  MPFR 변수 및 정밀도 설정
  mpfr_t pi, term, mom, sign, one, minus_one;
  const int precision = 16384; //정밀도 설정

  mpfr_init2(pi, precision);          // 결과를 저장할 pi 변수
  mpfr_init2(term, precision);        // 각 항을 계산할 term 변수
  mpfr_init2(mom, precision); // 분모 (1, 3, 5, ...)
  mpfr_init2(sign, precision);        // 부호
  mpfr_init2(one, precision);         //분자(1)
  mpfr_init2(minus_one, precision);   // -1

  mpfr_set_str(pi, line.c_str(), 10, MPFR_RNDN);         // pi 초기화
  mpfr_set_d(one, 1.0, MPFR_RNDN);        // 상수 1 설정
  mpfr_set_d(minus_one, -1.0, MPFR_RNDN); //상수 -1설정
  mpfr_set_d(sign, 1.0, MPFR_RNDN);

  string res(""), pi_last("");    // 파일에 담길 내용 담아줄 string, pi_value_last.txt에 적힐 pi값

  for (int i = start; i < max; i++) 
  {
    // 분모 계산 (2 * i + 1)
    mpfr_set_ui(mom, 2 * i + 1, MPFR_RNDN);

    // 항 계산: 1 / 분모
    mpfr_div(term, one, mom, MPFR_RNDN);

    // 부호 적용
    mpfr_mul(term, term, sign, MPFR_RNDN);

    // 4곱해주기
    mpfr_mul_ui(term, term, 4, MPFR_RNDN);
    mpfr_add(pi, term, pi, MPFR_RNDN);
    res += "n : " + to_string(i+1) + "\n";
    
    char *temp1 = new char[10020];
    mpfr_sprintf(temp1, "%.10000Rf", pi); // 소수점 이하 10000자리 출력

    string temp2(temp1);
    res += "pi : " + temp2 + "\n\n";
    // 부호 반전
    mpfr_mul(sign, minus_one, sign, MPFR_RNDN);
    if(i == max - 1)
    {
      pi_last.append(temp2);
    }
  }



  // 결과 출력
  cout << "Calculated π: ";
  mpfr_out_str(stdout, 10, 100, pi, MPFR_RNDN); // 소수점 100자리까지 출력
  cout << '\n';

  fstream outfile1("pi_value.txt", ios::app);
  if (!outfile1) {
    cout << "Can't open pi_value.txt\n";
    // 변수 해제
    mpfr_clear(pi);
    mpfr_clear(term);
    mpfr_clear(mom);
    mpfr_clear(sign);
    mpfr_clear(one);
    mpfr_clear(minus_one);
    return -1;
  }
  outfile1 << res;
  outfile1.close();
  fstream outfile2("pi_value_last.txt", ios::out);
  if (!outfile2) 
  {
    cout << "Can't open pi_value_last.txt\n";
    // 변수 해제
    mpfr_clear(pi);
    mpfr_clear(term);
    mpfr_clear(mom);
    mpfr_clear(sign);
    mpfr_clear(one);
    mpfr_clear(minus_one);
    return -1;
  }
  outfile2 << pi_last;
  outfile2.close();

  ofstream outfile3("pi_info.txt");
  if(!outfile3)
  {
    cout << "Can't open the pi_info.txt";
    // 변수 해제
    mpfr_clear(pi);
    mpfr_clear(term);
    mpfr_clear(mom);
    mpfr_clear(sign);
    mpfr_clear(one);
    mpfr_clear(minus_one);
  }

  outfile3 << to_string(max) << '\n';
  outfile3.close();

  // 변수 해제
  mpfr_clear(pi);
  mpfr_clear(term);
  mpfr_clear(mom);
  mpfr_clear(sign);
  mpfr_clear(one);
  mpfr_clear(minus_one); 


  return 0;
  // ./pi && ./pi_modify
  // cd - && git add . && git commit -m "." && git push origin main && cd Math/Pi
}