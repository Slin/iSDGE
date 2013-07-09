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

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.InputDevice;
import tv.ouya.console.api.OuyaController;

import java.io.File;


public class sgBase extends Activity
{
    ISDGEView mView;

    @Override protected void onCreate(Bundle icicle)
    {
        Log.i("restart", "onCreate");
        
        super.onCreate(icicle);
        mView = new ISDGEView(getApplication(), false, 16, 0);
        setContentView(mView);

        String apkFilePath = null;
        ApplicationInfo appInfo = null;
        PackageManager packMgmr = this.getPackageManager();
        try
        {
            appInfo = packMgmr.getApplicationInfo("com.android.isdge", 0);
        }
        catch (NameNotFoundException e)
        {
            e.printStackTrace();
            throw new RuntimeException("Unable to locate assets, aborting...");
        }
        apkFilePath = appInfo.sourceDir;
        ISDGELib.sgSetAPK(apkFilePath);

        OuyaController.init(this);
    }

    @Override protected void onPause()
    {
        Log.i("restart", "onPause");
        super.onPause();
        mView.onPause();
    }

    @Override public void onWindowFocusChanged(boolean gained)
    {
        Log.i("restart", "onWindowFocusChanged");
        //supposed to be a better time to pause/resume the game
        if(gained == false)
            onPause();
        else
            onResume();
    }

    @Override protected void onResume()
    {
        Log.i("restart", "onResume");
        super.onResume();
        mView.onResume();
    }

    @Override protected void onStop()
    {
        Log.i("restart", "onStop");
        super.onStop();
        ISDGELib.sgDestroy();
    }

    @Override
    public boolean onKeyDown(final int keyCode, KeyEvent event){
        //Get the player #
        int player = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());       
        boolean handled = false;

        //Handle the input
        switch(keyCode){
            case OuyaController.BUTTON_O:
                ISDGELib.sgSetGamepadKey(0, true);
                handled = true;
                break;

            case OuyaController.BUTTON_U:
                ISDGELib.sgSetGamepadKey(1, true);
                handled = true;
                break;

            case OuyaController.BUTTON_Y:
                ISDGELib.sgSetGamepadKey(2, true);
                handled = true;
                break;

            case OuyaController.BUTTON_A:
                ISDGELib.sgSetGamepadKey(3, true);
                finish();
                handled = true;
                break;
        }
        return handled;//? true : super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(final int keyCode, KeyEvent event){
        //Get the player #
        int player = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());       
        boolean handled = false;

        //Handle the input
        switch(keyCode){
            case OuyaController.BUTTON_O:
                ISDGELib.sgSetGamepadKey(0, false);
                handled = true;
                break;

            case OuyaController.BUTTON_U:
                ISDGELib.sgSetGamepadKey(1, false);
                handled = true;
                break;

            case OuyaController.BUTTON_Y:
                ISDGELib.sgSetGamepadKey(2, false);
                handled = true;
                break;

            case OuyaController.BUTTON_A:
                ISDGELib.sgSetGamepadKey(3, false);
                handled = true;
                break;
        }
        return handled;//? true : super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onGenericMotionEvent(final MotionEvent event)
    {
        //Get the player #
        int player = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());    

        //Joystick
        if((event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0)
        {
            float LS_X = event.getAxisValue(OuyaController.AXIS_LS_X);
            float LS_Y = event.getAxisValue(OuyaController.AXIS_LS_Y);
            float RS_X = event.getAxisValue(OuyaController.AXIS_RS_X);
            float RS_Y = event.getAxisValue(OuyaController.AXIS_RS_Y);
            float L2 = event.getAxisValue(OuyaController.AXIS_L2);
            float R2 = event.getAxisValue(OuyaController.AXIS_R2);

            ISDGELib.sgGamepadLeftJoy(LS_X, LS_Y);
            ISDGELib.sgGamepadRightJoy(RS_X, RS_Y);
            ISDGELib.sgGamepadLeftTrigger(L2);
            ISDGELib.sgGamepadRightTrigger(R2);
            return true;
        }

        //Touchpad
/*        if((event.getSource() & InputDevice.SOURCE_CLASS_POINTER) != 0)
        {
            switch(event.getAction())
            {
                case MotionEvent.ACTION_POINTER_DOWN:
                    Log.i("Input", "touch");
                    ISDGELib.sgTouchBegan(mView.getWidth()-(int)event.getX(), mView.getHeight()-(int)event.getY());
                break;

                case MotionEvent.ACTION_MOVE:
                    ISDGELib.sgTouchMoved(mView.getWidth()-(int)event.getX(), mView.getHeight()-(int)event.getY());
                break;

                case MotionEvent.ACTION_POINTER_UP:
                case MotionEvent.ACTION_CANCEL:
                    ISDGELib.sgTouchEnded(mView.getWidth()-(int)event.getX(), mView.getHeight()-(int)event.getY());
                break;

                default:
                    return false;
            }
            return true;
        }*/

        return false;
    }
}
