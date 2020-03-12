package com.example.proyecto1android;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import org.w3c.dom.Text;

import java.util.Set;

public class MainActivity extends AppCompatActivity {
    public static int Ticket=0;
    public static int ModoDeJuego=0;
    private static final String TAG = "DispositivosBT";
    ListView idlista;
    TextView LblTicket,LblModo;
    Button BtnJugar,BtnModo,BtnFichas;
    public static String EXTRA_DEVICE_ADDRESS = "device_address";
    public static String Direcc = null;
    private BluetoothAdapter mBtAdapter;
    private ArrayAdapter mPairedDevicesArrayAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        BtnJugar=(Button) findViewById(R.id.BtnJugar);
        BtnModo=(Button) findViewById(R.id.BtnModo);
        BtnFichas=(Button) findViewById(R.id.BtnFichas);
        LblTicket=(TextView)findViewById(R.id.LblTicket);
        LblModo=(TextView)findViewById(R.id.LblModo);
        BtnFichas.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                Intent i = new Intent(MainActivity.this, Monedas.class);
                i.putExtra(EXTRA_DEVICE_ADDRESS, Direcc);
                startActivity(i);
            }
          });
        BtnModo.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                if(Ticket<1 && ModoDeJuego==0)
                    Toast.makeText(getBaseContext(), "No Ha Comprado Tickets", Toast.LENGTH_SHORT).show();
                else{

                    Intent i = new Intent(MainActivity.this, ModoJuego.class);
                    i.putExtra(EXTRA_DEVICE_ADDRESS, Direcc);
                    startActivity(i);
                }
            }
        });
        BtnJugar.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                if(ModoDeJuego==0)
                    Toast.makeText(getBaseContext(), "No Ha Seleccionado Un Modo De Juego", Toast.LENGTH_SHORT).show();
                else{

                Intent i = new Intent(MainActivity.this, Jugar.class);
                i.putExtra(EXTRA_DEVICE_ADDRESS, Direcc);
                startActivity(i);
                }
            }
        });
        /*
        BtnManual.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                Intent i = new Intent(MainActivity.this, ControlDeCarro.class);
                i.putExtra(EXTRA_DEVICE_ADDRESS, Direcc);
                startActivity(i);
            }
        });*/

    }
    private void ActualizarInfo(){
        LblTicket.setText(Ticket+"");
        switch (ModoDeJuego){
            case 0:
                LblModo.setText("Sin Modo");
                break;
            case 1:
                LblModo.setText("Manual");
                break;
            case 2:
                LblModo.setText("Remoto");
                break;
        }

    }
    @Override
    public void onResume(){
        ActualizarInfo();
        super.onResume();
        //---------------------------------
        VerificarEstadoBT();

        // Inicializa la array que contendra la lista de los dispositivos bluetooth vinculados
        mPairedDevicesArrayAdapter = new ArrayAdapter(this,R.layout.namedispo);//<-<- PARTE A MODIFICAR >->->
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
