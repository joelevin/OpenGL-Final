class Window {
    public:
        ~Window();
        void InitWindow(int*, char**m, int, int, int, int);
        void MakeWindow(char*);
        void MakeSubWindow(int, int, int, int, int);
        void DestroyWindow();
        void SetActive();
        void ShowCursor(bool);
		void DisplayFunction(void (*callback)());
		void ReshapeFunction(void (*callback)(int, int));
		void WindowInit(void (*callback)());
		void KeyBoardFunction(void (*callback)(unsigned char ,int , int ));
		void SpecialKeyBoardFunction(void (*callback)(int ,int , int ));
		void IdleFunction(void (*callback)());
        int GetWindowID();

		class Camera {
    public:
        Camera();
        void SetPosition(float, float, float, float, float, 
            float, float, float, float);
        void Update();
        void MoveForward();
        void MoveBackward();
        void StrafeLeft();
        void StrafeRight();
        void Rotate(int, int);
        void RotateLeft();
        void RotateRight();
        float GetNewXPos(int direction);
        float GetNewZPos(int direction);
        enum {FORWARD, BACKWARD};

    //private:
        // motion speed variables
        float up, down;
        float left, right;
        float strafe_left, strafe_right;

        // position variables
        float eye_x, eye_y, eye_z;
        float center_x, center_y, center_z;
        float up_x, up_y, up_z;
        float xstrafe, ystrafe, zstrafe;
        float rotangle;

        void Move(float);
        void CalculateStrafe();
        void Strafe(float);
        void UpdatePosition(float, float, float);
        void RotateView(float, float, float, float);
};

		Camera *cam;

    private:
        int wndid;
};