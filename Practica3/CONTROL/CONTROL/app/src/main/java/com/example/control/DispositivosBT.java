package com.example.control;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.ListView;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.widget.Toast;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import java.util.Set;

public class DispositivosBT extends AppCompatActivity {
    private static final String TAG = "DispositivosBT";
    ListView idlista;
    Button BtnGuardar,BtnManual,BtnRutas;
    public static String EXTRA_DEVICE_ADDRESS = "device_address";
    public static String Direcc = null;
    private BluetoothAdapter mBtAdapter;
    private ArrayAdapter mPairedDevicesArrayAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dispositivos_bt);
        BtnGuardar=(Button) findViewById(R.id.BtnGuardar);
        BtnManual=(Button) findViewById(R.id.BtnManual);
        BtnRutas=(Button) findViewById(R.id.BtnRutas);

        BtnRutas.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                Intent i = new Intent(DispositivosBT.this, Recorrer.class);
                i.putExtra(EXTRA_DEVICE_ADDRESS, Direcc);
                startActivity(i);
            }
        });
        BtnGuardar.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                Intent i = new Intent(DispositivosBT.this, Guardar.class);
                i.putExtra(EXTRA_DEVICE_ADDRESS, Direcc);
                startActivity(i);
            }
        });
        BtnManual.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                Intent i = new Intent(DispositivosBT.this, ControlDeCarro.class);
                i.putExtra(EXTRA_DEVICE_ADDRESS, Direcc);
                startActivity(i);
            }
        });

    }

    @Override
    public void onResume(){
        super.onResume();
        //---------------------------------
        VerificarEstadoBT();

        // Inicializa la array que contendra la lista de los dispositivos bluetooth vinculados
        mPairedDevicesArrayAdapter = new ArrayAdapter(this, R.layout.namedispo);//<-<- PARTE A MODIFICAR >->->
        // Presenta los disposisitivos vinculados en el ListView
        idlista = (ListView) findViewById(R.id.idlista);
        idlista.setAdapter(mPairedDevicesArrayAdapter);
        idlista.setOnItemClickListener(mDeviceClickListener);
        // Obtiene el adaptador local Bluetooth adapter
        mBtAdapter = BluetoothAdapter.getDefaultAdapter();

        //------------------- EN CASO DE ERROR -------------------------------------
        //SI OBTIENES UN ERROR EN LA LINEA (BluetoothDevice device : pairedDevices)
        //CAMBIA LA SIGUIENTE LINEA POR
        //Set <BluetoothDevice> pairedDevices = mBtAdapter.getBondedDevices();
        //------------------------------------------------------------------------------

        // Obtiene un conjunto de dispositivos actualmente emparejados y agregua a 'pairedDevices'
        Set<BluetoothDevice> pairedDevices = mBtAdapter.getBondedDevices();

        // Adiciona un dispositivos previo emparejado al array
        if (pairedDevices.size() > 0)
        {
            for (BluetoothDevice device : pairedDevices) { //EN CASO DE ERROR LEER LA ANTERIOR EXPLICACION
                mPairedDevicesArrayAdapter.add(device.getName() + "\n" + device.getAddress());
            }
        }
    }


    // Configura un (on-click) para la lista
    private AdapterView.OnItemClickListener mDeviceClickListener = new AdapterView.OnItemClickListener() {
        public void onItemClick(AdapterView av, View v, int arg2, long arg3) {

            // Obtener la dirección MAC del dispositivo, que son los últimos 17 caracteres en la vista
            String info = ((TextView) v).getText().toString();
            String address = info.substring(info.length() - 17);
            Direcc=address;
            /*
            Intent i = new Intent(DispositivosBT.this, Guardar.class);
            i.putExtra(EXTRA_DEVICE_ADDRESS, address);
            startActivity(i);
            */
/*
            Intent i = new Intent(DispositivosBT.this, ControlDeCarro.class);
            Direcc=address;
            i.putExtra(EXTRA_DEVICE_ADDRESS, address);
            startActivity(i);*/

            // Realiza un intent para iniciar la siguiente actividad
            // mientras toma un EXTRA_DEVICE_ADDRESS que es la dirección MAC.
            //Intent i = new Intent(DispositivosBT.this, MainActivity.class);//<-<- PARTE A MODIFICAR >->->

        }
    };

    private void VerificarEstadoBT() {
        // Comprueba que el dispositivo tiene Bluetooth y que está encendido.
        mBtAdapter= BluetoothAdapter.getDefaultAdapter();
        if(mBtAdapter==null) {
            Toast.makeText(getBaseContext(), "El dispositivo no soporta Bluetooth", Toast.LENGTH_SHORT).show();
        } else {
            if (mBtAdapter.isEnabled()) {
                Log.d(TAG, "...Bluetooth Activado...");
            } else {
                //Solicita al usuario que active Bluetooth
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);

            }
        }
    }
}
