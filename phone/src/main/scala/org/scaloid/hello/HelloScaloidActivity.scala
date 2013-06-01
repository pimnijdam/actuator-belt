package org.scaloid.hello

import org.scaloid.common._
import android.graphics.Color

class HelloScaloidActivity extends SActivity {

    def f () = toast("hoi van f")

    onCreate {
        contentView = new SVerticalLayout {
            style {
                case b: SButton => b.textColor(Color.RED).onClick(toast("actuator-belt!"))
                case t: STextView => t.textSize(10 dip)
                case v => v.backgroundColor(Color.YELLOW)
            }

            STextView("Waiting for bluetooth devices...")
            SButton(R.string.red)
        }.padding(20 dip)
    }
}
