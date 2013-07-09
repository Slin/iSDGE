/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.isdge;

// Wrapper for native library

public class ISDGELib {

     static {
         System.loadLibrary("sgLib");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void sgInit(int width, int height);
     
     public static native void sgDraw();
     public static native void sgDestroy();
     public static native void sgSetAPK(String path);
     public static native void sgSetGamepadKey(int key, boolean state);
     public static native void sgGamepadLeftJoy(float posx, float posy);
     public static native void sgGamepadRightJoy(float posx, float posy);
     public static native void sgGamepadLeftTrigger(float val);
     public static native void sgGamepadRightTrigger(float val);
     public static native void sgTouchBegan(int posx, int posy);
     public static native void sgTouchMoved(int posx, int posy);
     public static native void sgTouchEnded(int posx, int posy);
}
