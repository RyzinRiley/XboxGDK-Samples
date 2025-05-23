  ![](./media/image1.png)

#   MP4Reader 샘플(DirectX 12)

*이 샘플은 Microsoft 게임 개발 키트 미리 보기(2019년 11월)와
호환됩니다.*

# 

# 설명

이 샘플에서는 미디어 파운데이션 원본 판독기를 사용하여 H264 비디오
스트림이 포함된 MP4 파일을 읽고 하드웨어 가속을 사용하여 디코딩하는
방법을 보여 줍니다. XAudio2 및 WASAPI를 사용하여 오디오 스트림을
디코딩하는 방법도 보여 줍니다. 이 샘플은 H264 비디오 디코딩을 기존
동영상 재생 파이프라인에 통합해야 하는 타이틀에 특히 유용합니다.

이 샘플은 비디오 원본 프레임 속도로 비디오 재생을 동기화하는 것을
지원하지 않기 때문에 동영상 재생 해결 방법을 보여 주기 위한 것은
아닙니다.

![](./media/image3.png)

# 샘플 빌드하기

Xbox One 개발 키트를 사용하는 경우 활성 솔루션 플랫폼을
Gaming.Xbox.XboxOne.x64로 설정하세요.

Project Scarlett을 사용하는 경우 활성 솔루션 플랫폼을
Gaming.Xbox.Scarlett.x64로 설정하세요.

*자세한 내용은 GDK 문서에서* 샘플 실행하기*를 참조하세요.*

# 샘플 사용하기

이 샘플은 다음 컨트롤을 사용합니다.

| 작업                                         |  게임패드              |
|----------------------------------------------|-----------------------|
| 종료                                         |  보기 단추             |

# 구현 참고 사항

이 샘플은 Media\\Videos 폴더에서 H264 비디오 스트림을 포함하는 MPEG 4
파일을 읽고, 디코딩된 프레임이 생성되는 즉시 화면에 렌더링합니다.

이 샘플은 하드웨어 가속 디코딩만 사용하며, H.264 비디오 스트림의
소프트웨어 디코딩은 지원하지 않습니다. 오디오 스트림은 Microsoft Media
Foundation을 사용하여 디코딩되고 XAudio2 또는 WASAPI를 사용하여 오디오를
렌더링하도록 구성할 수 있습니다. 이 기능은 MP4Reader.h 위에서 전처리기가
정의하는 항목을 수정하여 제어합니다.

> //
>
> // 이러한 두 가지 정의 중 하나를 사용하여 두 가지 다른 기술이 작동하는
> 방식을 확인합니다.
>
> //
>
> //#define USE_XAUDIO2
>
> #define USE_WASAPI

vs.

> //
>
> // 이러한 두 가지 정의 중 하나를 사용하여 두 가지 다른 기술이 작동하는
> 방식을 확인합니다.
>
> //
>
> #define USE_XAUDIO2
>
> //#define USE_WASAPI

# 알려진 문제

없음

# 개인정보처리방침

샘플을 컴파일하고 실행할 때 샘플의 사용을 추적하는 데 도움이 되도록 샘플
실행 파일의 파일 이름이 Microsoft에 전송됩니다. 이 데이터 수집을
옵트아웃하려면 Main.cpp에서 \"샘플 사용 원격 분석\"이라고 레이블이
지정된 코드 블록을 제거할 수 있습니다.

Microsoft의 일반 개인정보취급방침에 대한 자세한 내용은 [Microsoft
개인정보처리방침](https://privacy.microsoft.com/en-us/privacystatement/)을
참조하세요.
