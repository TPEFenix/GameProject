#include <windows.h>
#include <vector>


namespace game_framework {
    struct BitMapRectangle
    {
        int X_int = 0;
        int Y_int = 0;
        double X = 0;
        double Y = 0;
        int Width = 0;
        int Height = 0;
    };
    class BitmapPicture : CMovingBitmap
    {
        friend class CDDraw;
     public:
        BitmapPicture();
        ~BitmapPicture();
        vector<vector<bool> > EffectRect;
        BitMapRectangle Rect;//圖片材質矩形 掌管座標跟長寬，會在讀取檔案時設定完成
        bool visable = false;
        void LoadTexture(char *, bool,COLORREF = CLR_INVALID);
        void OnUpdate();
        void Draw(int CurrentLayer, int TargetLayer);
        void Draw(int CurrentLayer, int TargetLayer,CMovingBitmap &Mask);
        bool CanCollision = false;
    };
}

