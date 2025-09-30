# 코딩 가이드라인 (실무형)

## 1) 폴더 구조 원칙
- `BSW/`: 하드웨어 의존 초기화 코드와 ISR, 저수준 드라이버
- `ReadAdc/`, `ReadSonic/`: 센서 값 취득 모듈(데이터 제공)
- `Servo/`, `LCDControl/`: 디바이스 제어 래퍼
- `VehicleControl/`: 비즈니스/제어 로직(센서 -> 판단 -> 액추에이터)
- `main.c`: 초기화, 주기 Task 분배(스케줄링은 ISR에서 호출)

## 2) 헤더(.h) 작성 규칙
- 공개 API만 노출하고, 무거운 의존성은 .c에서 `#include`
- 표준 타입은 `stdint.h`만 포함(AVR 레지스터 접근은 .c에서 필요 시 포함)
- 전방 선언을 활용하여 결합도 최소화
- 예) `Vehicle_Control.h`는 상수/extern/프로토타입만 제공

## 3) 전역 변수(데이터 공유)
- “소유 모듈”의 .c에서 정의, .h에서는 최소한으로 `extern` 공개
- 인터럽트 공유 변수는 `volatile` 사용, 임계영역 접근은 ISR-메인 충돌 주의
- 센서 원시값(`ReadAdc`) → 가공된 상태(`ReadSonic`의 `distance_cm[]`) → 제어 로직(`VehicleControl`) 순서로 흐름 유지

## 4) ISR와 주기 Task
- ISR은 짧게: 시간계수/플래그만 업데이트, 무거운 연산은 Task에서
- 타이머 ISR에서 5/20/60/1000ms 분기, `task_Xms()` 함수 호출
- `BSW/ISR.h`에는 실제 사용 Task 프로토타입만 유지

## 5) 제어 로직
- `VehicleControl`은 공개 함수로 `Speed_Control()`, `Steering_Control()`, `check_buttons()` 등 제공
- 상태 전이(예: `mode`)는 센서 모듈에서 갱신, 변경 시 UI(LCD) 업데이트는 한 번만
- 하드웨어 제어는 래퍼 함수 사용(`Servo_SetAngle`, `Motor_Speed` 등)

## 6) 네이밍/스타일
- 함수: 동사/행위형, 변수: 의미 중심 풀네임 사용
- 상수/매크로는 대문자 + 언더스코어
- 복잡 분기 전 Early Return, 깊은 중첩 금지

## 7) 컴파일 경량화
- .h에서 `F_CPU`, `<avr/io.h>`, `<util/delay.h>` 등은 지양
- 동일 모듈 .c에서만 필요한 헤더 포함(재빌드 범위 축소)

## 8) 예시 체크리스트
- [x] 헤더 순환참조 제거 (`Vehicle_Control.h`의 self-include 제거)
- [x] `UART.h`의 불필요 include 제거, `stdint.h`만 유지
- [x] `Sonic.h`는 API/상수만 노출, 무거운 include는 `Sonic.c`로 이동
- [x] `ISR.h` Task 프로토타입 정합성 유지 