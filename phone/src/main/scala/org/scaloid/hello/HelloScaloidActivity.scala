package org.scaloid.hello

import org.scaloid.common._
import android.graphics.Color
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Intent
import java.io.OutputStream
import java.util.UUID

class HelloScaloidActivity extends SActivity
{
    var mText             : STextView        = null
    var mBluetoothAdapter : BluetoothAdapter = null
    var mBluetoothDevice  : BluetoothDevice  = null
    var mmSocket          : BluetoothSocket  = null
    var mmOutputStream    : OutputStream     = null
    override implicit val tag = LoggerTag("BELT")

    // def doFilter ((acc, d)) = if ("linvor" == d.asInstanceOf[BluetoothDevice].getName()) d; else acc

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

                //val names = arr.map(d => d.asInstanceOf[BluetoothDevice].getName())
                //r         = names.reduceLeft(_+_)

                val arr   = pairedDevices.toArray
                mBluetoothDevice = arr.foldLeft(mBluetoothDevice)((acc, d) => if ("linvor" == d.asInstanceOf[BluetoothDevice].getName()) d.asInstanceOf[BluetoothDevice]; else acc)
            }
        } else
        {
            r = "no bluetoothadapter"
        }
                
        info("done, return r")

        mText.text = r
    }

    def openBlueTooth()
    {
        if (mBluetoothDevice != null)
        {
            val uuid = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb")
            mmSocket = mBluetoothDevice.createRfcommSocketToServiceRecord(uuid) 
            mmSocket.connect()
            mmOutputStream = mmSocket.getOutputStream();
            mmOutputStream.write('A')
        }
    }

    def buttonClicked () =
    {
        findBlueTooth()
        openBlueTooth()
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
