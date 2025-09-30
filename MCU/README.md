# AVR 프로젝트 구조 및 코딩 가이드 (요약)

이 프로젝트는 Atmel Studio(AVR-GCC) 기반으로, 실무에서 유지보수가 쉬운 구조와 일관된 코딩 규칙을 따릅니다. 자세한 규칙은 `docs/CODING_GUIDELINES.md`를 참고하세요.

## 폴더 구조

```
GccApplication1/
├─ BSW/                # Board Support/Driver 초기화 (ISR, UART, ADC_Init, GPIO_Init, Servo_Init, Sonic_Init 등)
├─ ReadAdc/            # ADC 변환 및 채널 스케줄링(데이터 제공 모듈)
├─ ReadSonic/          # 초음파 센서(거리 측정, 모드 업데이트)
├─ Servo/              # 서보/모터 제어 래퍼
├─ LCDControl/         # LCD 드라이버 및 화면 표시 유틸
├─ VehicleControl/     # 차량 제어 로직(속도, 조향, 버튼 처리, 모드 반영)
├─ main.c              # 엔트리포인트, 초기화 & 주기 Task 호출(타이머 ISR 기반)
└─ ...
```

## 핵심 원칙
- 헤더(.h)는 “공개 API”만 노출: 표준 타입(`stdint.h`)만 포함하고, 무거운 의존성은 .c에서 포함
- 전역 변수는 “소유 모듈”의 .c에서 정의하고, 필요한 최소 범위로 `extern` 공개
- 인터럽트는 짧게: 복잡한 로직은 주기 Task로 분리하여 처리
- 모듈 간 의존성 최소화: 상호 include 금지, 상위에서 조립

## 주기 Task 설계
- `ISR(TIMER2_COMPA_vect)`에서 5ms/20ms/60ms/1000ms 주기를 분기 호출
- 각 Task는 빠르게 끝나야 하며, 블로킹 작업 금지(Delay 최소화)

## 빌드 포인트
- `main.c`에 모듈 초기화와 Task 함수가 존재
- `BSW/ISR.h`는 실제 사용 중인 Task 프로토타입만 선언됨

자세한 예시와 규칙은 `docs/CODING_GUIDELINES.md` 파일을 참고하세요. 