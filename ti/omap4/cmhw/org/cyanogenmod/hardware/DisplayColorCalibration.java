/*
 * Copyright (C) 2016 The CyanogenMod Project
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

 /*
  * According to the OMAP color phase correction documentation,
  * this happens:
  *
  * +-    -+         +-        -+   +-   -+
  * | Rout |         | Rr Rg Rb |   | Rin |
  * |      |    1    |          |   |     |
  * | Gout | = --- * | Gr Gg Gb | * | Gin |
  * |      |   256   |          |   |     |
  * | Bout |         | Br Bg Bb |   | Bin |
  * +-    -+         +-        -+   +-   -+
  *
  * So to get the unmodified color, the values for Rr, Gg and Bb should
  * be set to 256, all others to 0. The values are 10 bit signed.
  *
  * This interface is exposed via sysfs and takes one line in the format:
  * Rr Rg Rb Gr Gg Gb Br Bg Bb
  */

package org.cyanogenmod.hardware;

import org.cyanogenmod.internal.util.FileUtils;

public class DisplayColorCalibration {
    private static final String MGR_PATH = getManagerPath();

    private static final String COLOR_FILE = MGR_PATH + "cpr_coef";
    private static final String COLOR_FILE_CTRL = MGR_PATH + "cpr_enable";

    private static int last_rgb[] = { 256, 256, 256 };

    private static String getManagerPath() {
        final String sysfs_root = "/sys/devices/platform/omapdss/";
        final String disp_name = FileUtils.readOneLine(sysfs_root + "display0/name");

        String mgr_path = "";

        for (int mgr_idx = 0; mgr_idx < 3; mgr_idx++) {
            mgr_path = sysfs_root + "manager" + mgr_idx + "/";
            if (disp_name.equals(FileUtils.readOneLine(mgr_path + "display"))) {
                break;
            }
        }

        return mgr_path;
    }

    public static boolean isSupported() {
        return true;
    }

    public static int getMaxValue()  {
        return 256;
    }

    public static int getMinValue()  {
        return 0;
    }

    public static int getDefValue() {
        return getMaxValue();
    }

    public static String getCurColors()  {
        return last_rgb[0] + " " + last_rgb[1] + " " + last_rgb[2];
    }

    public static boolean setColors(String colors)  {
        String rgb[] = colors.split(" ");
        String cpr_coefs = rgb[0] + " 0 0 0 " + rgb[1] + " 0 0 0 " + rgb[2];

        if (last_rgb[0] == Integer.parseInt(rgb[0]) &&
                last_rgb[1] == Integer.parseInt(rgb[1]) &&
                last_rgb[2] == Integer.parseInt(rgb[2])) {
            return true;
        }

        if (!FileUtils.writeLine(COLOR_FILE, cpr_coefs)) {
            return false;
        }

        last_rgb[0] = Integer.parseInt(rgb[0]);
        last_rgb[1] = Integer.parseInt(rgb[1]);
        last_rgb[2] = Integer.parseInt(rgb[2]);

        return FileUtils.writeLine(COLOR_FILE_CTRL, "1");
    }
}
