
package org.lineageos.settings.doze;

import android.os.Bundle;
import android.preference.PreferenceActivity;

public class DozeSettingsActivity extends PreferenceActivity {

    private static final String TAG_DOZE = "doze";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getFragmentManager().beginTransaction().replace(android.R.id.content,
                new DozeSettingsFragment(), TAG_DOZE).commit();
    }
}
