package telly.EffectsManager;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;

/**
 * @author cristian
 * @version 1.0
 */
public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LinearLayout container = new LinearLayout(this);
        container.setOrientation(LinearLayout.VERTICAL);

        /* Button ryu = new Button(this);
        ryu.setText("Launch first shader test");
        ryu.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(MainActivity.this, EffectManagerActivity.class));
            }
        }) */;

        Button blanca = new Button(this);
        blanca.setText("Launch camera test");
        blanca.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(MainActivity.this, SimpleCameraExample.class));
            }
        });

        //container.addView(ryu);
        container.addView(blanca);

        setContentView(container);
    }
}
