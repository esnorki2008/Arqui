package com.example.control;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class ControlDeCarro extends AppCompatActivity {

    //1)
    Button BtnArriba, BtnAbajo,BtnIzquierda,BtnDerecha,BtnBarrer;
    TextView Nose;
    //-------------------------------------------
    public static String EXTRA_DEVICE_ADDRESS = "device_address";
    Handler bluetoothIn;
    final int handlerState = 0;
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private StringBuilder DataStringIN = new StringBuilder();
    private ControlDeCarro.ConnectedThread MyConexionBT;
    // Identificador unico de servicio - SPP UUID
    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    // String para la direccion MAC
    private static String address = null;
    public static String direcc = null;
//-------------------------------------------

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_control_de_carro);
        //2)

        BtnArriba = (Button) findViewById(R.id.BtnArriba);
        BtnIzquierda = (Button) findViewById(R.id.BtnIzquierda);
        BtnDerecha = (Button) findViewById(R.id.BtnDerecha);
        BtnAbajo = (Button) findViewById(R.id.BtnAbajo);
        BtnBarrer = (Button) findViewById(R.id.BtnBarrer);



        bluetoothIn = new Handler() {
            public void handleMessage(android.os.Message msg) {
                if (msg.what == handlerState) {
                    String readMessage = (String) msg.obj;
                    DataStringIN.append(readMessage);

                    int endOfLineIndex = DataStringIN.indexOf("#");

                    if (endOfLineIndex > 0) {
                        String dataInPrint = DataStringIN.substring(0, endOfLineIndex);
                        //IdBufferIn.setText("Dato: " + dataInPrint);//<-<- PARTE A MODIFICAR >->->
                        DataStringIN.delete(0, DataStringIN.length());
                    }
                }
            }
        };

        btAdapter = BluetoothAdapter.getDefaultAdapter(); // get Bluetooth adapter
        VerificarEstadoBT();
        BtnArriba.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                InfoEnvia("n");
            }
        });

        BtnDerecha.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                InfoEnvia("o");
            }
        });
        BtnIzquierda.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                InfoEnvia("e");
            }
        });
        BtnAbajo.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                InfoEnvia("s");
            }
        });

        BtnBarrer.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                InfoEnvia("t");
            }
        });


        /*IdDesconectar.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                if (btSocket!=null)
                {
                    try {btSocket.close();}
                    catch (IOException e)
                    { Toast.makeText(getBaseContext(), "Error", Toast.LENGTH_SHORT).show();;}
                }
                finish();
            }
        });*/
    }
    void InfoEnvia(String id){
        if (address == null) {
            System.out.println("Error de Conexion");
        } else {
            System.out.println("Enviando "+id);
            MyConexionBT.write(id);
        }
    }

    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException
    {
        //crea un conexion de salida segura para el dispositivo
        //usando el servicio UUID
        return device.createRfcommSocketToServiceRecord(BTMODULEUUID);
    }

    @Override
    public void onResume()
    {
        super.onResume();
        //Consigue la direccion MAC desde DeviceListActivity via intent
        Intent intent = getIntent();
        //Consigue la direccion MAC desde DeviceListActivity via EXTRA
        address = intent.getStringExtra(DispositivosBT.EXTRA_DEVICE_ADDRESS);//<-<- PARTE A MODIFICAR >->->

        direcc=address;
        //Setea la direccion MAC
        Log.i("STR", address);
        BluetoothDevice device = btAdapter.getRemoteDevice(address);

        try
        {
            btSocket = createBluetoothSocket(device);
        } catch (IOException e) {
            Toast.makeText(getBaseContext(), "La creacción del Socket fallo", Toast.LENGTH_LONG).show();
        }
        // Establece la conexión con el socket Bluetooth.
        try
        {
            btSocket.connect();
        } catch (IOException e) {
            try {
                Toast.makeText(getBaseContext(), "No Se Pudo Conectar", Toast.LENGTH_LONG).show();
                btSocket.close();
            } catch (IOException e2) {}
        }
        MyConexionBT = new ControlDeCarro.ConnectedThread(btSocket);
        MyConexionBT.start();
    }

    @Override
    public void onPause()
    {
        super.onPause();
        try
        { // Cuando se sale de la aplicación esta parte permite
             //que no se deje abierto el socket
            btSocket.close();
        } catch (IOException e2) {}
    }

    //Comprueba que el dispositivo Bluetooth Bluetooth está disponible y solicita que se active si está desactivado
    private void VerificarEstadoBT() {

        if(btAdapter==null) {
            Toast.makeText(getBaseContext(), "El dispositivo no soporta bluetooth", Toast.LENGTH_LONG).show();
        } else {
            if (btAdapter.isEnabled()) {
            } else {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);
            }
        }
    }

    //Crea la clase que permite crear el evento de conexion
    private class ConnectedThread extends Thread
    {
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket)
        {
            InputStream tmpIn = null;
            OutputStream tmpOut = null;
            try
            {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }
            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }
        String Lectura="";
        public void run()
        {
            byte[] buffer = new byte[256];
            int bytes;
            int z;
            // Se mantiene en modo escucha para determinar el ingreso de datos

                /*while (true) {
                    try {
                        bytes = mmInStream.read(buffer);            //read bytes from input buffer
                        String readMessage = new String(buffer, 0, bytes);
                        // Send the obtained bytes to the UI Activity via handler

                        Lectura=Lectura+readMessage;
                        if(readMessage.contains("$")){

                            Log.i("STR", Lectura);
                            Lectura="";

                        }
                        //if(read)
                    } catch (IOException e) {
                        break;
                    }
                }*/




                    /*bytes = mmInStream.read(buffer);
                    z= mmInStream.read(buffer);
                    String readMessage = new String(buffer, 0, bytes);
                    Lectura=Lectura+readMessage;
                    Log.d("SalidaByte", bytes+"******");
                    Log.d("SalidaRead", readMessage+"******");
                    if(readMessage.equals("$")){
                        Log.d("myTag", Lectura);
                        Lectura="";
                    }
                    // Envia los datos obtenidos hacia el evento via handler
                    bluetoothIn.obtainMessage(handlerState, bytes, -1, readMessage).sendToTarget();*/
            }

        //Envio de trama
        public void write(String input)
        {
            try {
                mmOutStream.write(input.getBytes());
            }
            catch (IOException e)
            {
                //si no es posible enviar datos se cierra la conexión
                Toast.makeText(getBaseContext(), "La Conexión fallo", Toast.LENGTH_LONG).show();
                finish();
            }
        }

    }
}
