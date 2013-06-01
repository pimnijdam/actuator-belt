package org.scaloid.hello

import org.scaloid.common._
import android.graphics.Color

class HelloScaloidActivity extends SActivity {

    var mText : STextView = null

    def findBlueTooth () =
    {
        toast("op zoek")
        mText.text = "gevonden"
    }

    def buttonClicked () =
    {
        mText = findBlueTooth()
    }

    onCreate {
        contentView = new SVerticalLayout {
            style {
                case b: SButton => b.textColor(Color.RED).onClick(buttonClicked)
                case t: STextView => t.textSize(10 dip)
                case v => v.backgroundColor(Color.YELLOW)
            }

            mText = STextView("Waiting for bluetooth devices...")
            SButton(R.string.red)
        }.padding(20 dip)
    }
}
