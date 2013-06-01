package org.scaloid.hello

import org.scaloid.common._
import android.graphics.Color
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Intent
import android.hardware.{Sensor, SensorEvent, SensorEventListener, SensorManager}
import android.content.Context
import java.io.OutputStream
import java.util.UUID

class HelloScaloidActivity extends SActivity
{
    var mText             : STextView        = null
    var mBluetoothAdapter : BluetoothAdapter = null
    var mBluetoothDevice  : BluetoothDevice  = null
    var mmSocket          : BluetoothSocket  = null
    var mmOutputStream    : OutputStream     = null
    override implicit val tag                = LoggerTag("BELT")
    var mSensorManager    : SensorManager    = null

    private final val mListener = new SensorEventListener()
    {

        var direction : Int = 0

        def onSensorChanged(sensor: SensorEvent)
        {
            val degrees = sensor.values(0)
            val f = degrees * 8 / 360
            val d = f.round % 8
            if (d != direction)
            {
                info("new direction " + f + " --> " + d)

                send(('a' + direction).toChar)
                send(('A' + d).toChar)
                direction = d
            }
        }

        def onAccuracyChanged(sensor: Sensor, accuracy: Int) { }
    }

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
        }
    }

    def send(c : Char)
    {
        if (mmOutputStream != null) mmOutputStream.write(c)
    }

    def init () =
    {
        findBlueTooth()
        openBlueTooth()
    }

    def start () =
    {
        send('J')
    }

    def stop () =
    {
        send('j')
    }

    onCreate
    {
        contentView = new SVerticalLayout
        {
            style
            {
                case b: SButton => b.textColor(Color.RED)
                case t: STextView => t.textSize(10 dip)
                case v => v.backgroundColor(Color.YELLOW)
            }
            mText = STextView("Waiting for bluetooth devices...")
            SButton("init").onClick(init)
            SButton("start").onClick(start)
            SButton("stop").onClick(stop)
        }.padding(20 dip)

        mSensorManager = getSystemService(Context.SENSOR_SERVICE).asInstanceOf[SensorManager]
        val sensors = mSensorManager.getSensorList(Sensor.TYPE_ORIENTATION)
        val it = sensors.iterator()
        while (it.hasNext) {
          mSensorManager.registerListener(mListener, it.next(),
            SensorManager.SENSOR_DELAY_GAME)
        }
    }
}
