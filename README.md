# Smart-Temperature-System

## 1. 프로젝트 소개

### 원인
 * 비 오는 날 외부 활동으로 양말이 젖거나, 세탁 후 건조가 충분히 이루어지지 않는 문제 발생

### 목표
 * 임베디드 시스템 개발 역량 향상을 위한 온도 제어 시스템 구현
 * STM32 단독 제어 버전과 Raspberry Pi 연동 버전 구현

---

## 2. 시스템 아키텍쳐

![stm32-raspberrypi](images\stm32-raspberrypi_temp_system.png)
![stm32](images\stm32_temp_system.png)

---

## 3. 트러블 슈팅

* DHT11 및 DS18B20 통신 중 타이밍 오류 발생 -> FreeRtos 사용간 임계영역(critical)으로 타이밍 보장 및 tim로 delay 구현
* 힙 메모리 부족으로 task 생성 실패 -> STM32CubeMX의 메모리 설정을 조정

---

## 4. 결과

![stm32-raspberrypi시스템사진](images\STM32-RaspberryPi.png)
시연 영상 : https://youtu.be/RZiizqCIeg0

* UART 기반 STM32–Raspberry Pi 통신 구현
* Linux C, FIFO를 활용한 프로세스 간 통신 및 GTK GUI 연동

![stm32시스템사진](images\STM32.png)
시연 영상 : https://youtu.be/ANTitgE3GZw

* STM32 주변장치(ADC, I2C, SPI, UART)를 활용한 센서 입력 및 상태 표시 구현
* FreeRTOS 기반 태스크 분리 및 시스템 상태 관리

---