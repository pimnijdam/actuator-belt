package org.scaloid.hello

import org.scaloid.common._
import android.graphics.Color
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Intent

class HelloScaloidActivity extends SActivity
{
    var mText             : STextView        = null
    var mBluetoothAdapter : BluetoothAdapter = null
    var mBluetoothDevice  : BluetoothDevice  = null
    override implicit val tag = LoggerTag("BELT")

    //def toName (d : Object) = { d.asInstanceOf[BluetoothDevice].getName() }

    def findBlueTooth () =
    {
        var r = ""


        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
        if (mBluetoothAdapter != null)
        {
            info("is bluetooth enabled?")

            if(!mBluetoothAdapter.isEnabled())
            {
                info("enabling bluetooth...")
                val enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBluetooth, 0);
            }
            info("bluetooth is enabled, get bondend devices")

            var pairedDevices = mBluetoothAdapter.getBondedDevices();
            if(pairedDevices.size() > 0)
            {
                info("compose list of devices")

                val arr   = pairedDevices.toArray
                val names = arr.map(d => d.asInstanceOf[BluetoothDevice].getName())
                r         = names.reduceLeft(_+_)
            }
        } else
        {
            r = "mislukt"
        }
                
        info("done, return r")

        mText.text = r
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
