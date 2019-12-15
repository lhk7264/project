# 장애인주차구역 관리시스템

[장애인주차구역 불법 장기주차 '눈살'](https://www.jemin.com/news/articleView.html?idxno=632094)


[장애인 전용주차구역에 불법 주차 제발 하지 마세요](http://www.ilyoseoul.co.kr/news/articleView.html?idxno=350156)

#### 장애인 차량 번호가 저장되 있는 db와 주차된 차량의 db를 비교하여 장애인 차량인지 아닌지 확인하는 시스템.

#### 장애인 주차 공간에 주자되는 차량의 번호를 db에 기록하고 번호판을 캡쳐해 서버에 저장한다.

#### 장애인 차량이 아닌 불법주차일 경우 차량의 번호판을 캡쳐한 이미지가 서버에 저장되어 관리가 가능하게된다.



## 결과

### ocr 인식이 완벽하게 되지 않고있다. 학습데이터를 추가하는 방법을 찾아 인식률을 높이고 한글화를 완벽하게 구성해야할 필요가있다.

![initial](https://user-images.githubusercontent.com/49156302/70866508-f77a1d80-1fad-11ea-90ee-5fc1f905af22.JPG)

### 인식된 번호를 txt파일로 저장하도록 하였고 위반차량일 경우에는 번호판을 crop하여 서버내에 남겨두었다.
![initial](https://user-images.githubusercontent.com/49156302/70866509-f943e100-1fad-11ea-9d2b-882c8a7ff938.JPG)

### 장애인 차량의 유무를 알수있게 장애인 차량의 번호를 db에 저장하였다.
![initial](https://user-images.githubusercontent.com/49156302/70866510-fba63b00-1fad-11ea-891d-7adb2328219f.JPG)

### 인식된 차량의 번호를 db에 실시간으로 저장.
![initial](https://user-images.githubusercontent.com/49156302/70866511-fcd76800-1fad-11ea-8373-6b09d8e4d86d.JPG)


# **Incomplete**

#### 인식된 번호의 db와 장애인 차량의 db Table를 비교하여 장애인 차량의 유무를 확인하는 부분을 시스템으로 만들고 싶었지만 db로 저장된 값을 User가 직접 대조하는 것 까지만 구현하여 아쉬운 부분이 이다. 
#### 추후에 Table 비교후 위반 차량만 따로 db로 저장하여 위반 차량 리스트를 구성하고 위반차량의 번호를 crop하여 알리는 시스템으로 업그레이드해야 완벽한 시스템이 완성된하고 생각한다. 기간 내에 프로젝트를 완벽하게 구현하지 못한부분은 tesseract ocr을 아예 모르는 상태에서 사용하게 되어 시간을 소모한 부분과 db를 생성하고 사용하는 과정에서 미숙함이 더해졌다. 다음 프로젝트를 진행하면서 시간을 만들어 구현하지 못한 기능을 추가할 예정이다.

#### github 사용에 익숙하지 않은점이 굉장히 당황스럽게 다가왔다... 프로젝트를 진행하는 과정에서 commit과 push를 습관화 하지 않으점 또한 git사용의 미숙함을 많이 느끼게 하는 부분이였다.
