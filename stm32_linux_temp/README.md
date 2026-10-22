# Stm32 - raspberry pi

## 1. 기술 및 구현 기능

### stm32
| 사용 기술 | 구현 기능 |
|------|------|
| **freertos** | 기능별 task 분리 및 멀티 태스킹 구현 |
| **I2C** | SSD1306 OLED로 실시간 정보 출력 |
| **uart** | 명령어를 통해 raspberry pi와 통신 |

### raspberry pi
| 사용 기술 | 구현 기능 |
|------|------|
| **gui** | stm32의 실시간 정보 출력 및 사용자 입력 |
| **fifo** | 프로세서간 통신으로 gui와 uart 기능 연결 |
| **thread** | uart rx, tx 멀티 태스킹 구현 |

---
## 2. 하드웨어 구성

### 사용 부품

* stm32f103c8t6
* raspberry pi 4 
* dht11 온도 센서
* SSD1306 OLED 디스플레이
* 릴레이 모듈
* LED
* 택트 스위치

---

## 3. 결과

![시스템 사진](images\STM32-RaspberryPi.png)

시연 영상 : https://youtu.be/RZiizqCIeg0

---