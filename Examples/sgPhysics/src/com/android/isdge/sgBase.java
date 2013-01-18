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

import java.io.File;


public class sgBase extends Activity {

    ISDGEView mView;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new ISDGEView(getApplication(), false, 24, 0);
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
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
}
