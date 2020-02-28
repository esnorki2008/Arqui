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
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class Guardar extends AppCompatActivity {

    //1)
    Button BtnRecorrer, BtnLimpiar;
    TextView Seleccionado;
    ListView ListaSuperior,ListaInferior;
    //-------------------------------------------
    Handler bluetoothIn;
    final int handlerState = 0;
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private StringBuilder DataStringIN = new StringBuilder();
    private Guardar.ConnectedThread MyConexionBT;
    // Identificador unico de servicio - SPP UUID
    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    // String para la direccion MAC
    private static String address = null;
//-------------------------------------------
    String Titulos[]=new String[3];
    String Contenido[][]=new String[3][10];

    private void CaraInfo(String Dita){
        Log.i("Q",Dita);
        String[] Ru=Dita.split("\\}");
        for(int i=0;i<3;i++){


            String[] Contend=Ru[i].split(",");
            //Titulos[i]=Contend[0];
            for(int j=0;j<20;j++)
                Log.d("STR", Contend.length+"");
        }
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_guardar);
        //2)
        ListaSuperior=(ListView)findViewById(R.id.LstRutas);
        ListaSuperior=(ListView)findViewById(R.id.LstContenido);
        BtnLimpiar = (Button) findViewById(R.id.BtnLimpiar);
        BtnRecorrer = (Button) findViewById(R.id.BtnRecorrer);



        btAdapter = BluetoothAdapter.getDefaultAdapter(); // get Bluetooth adapter
        VerificarEstadoBT();

        BtnLimpiar.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                InfoEnvia("info");
            }
        });

        BtnRecorrer.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                InfoEnvia("info");
            }
        });
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
        return device.createRfcommSocketToServiceRecord(BTMODULEUUID);
    }

    @Override
    public void onResume()
    {
        super.onResume();
        Intent intent = getIntent();
        // Log.i("STR", address);
        //address = intent.getStringExtra(DispositivosBT.EXTRA_DEVICE_ADDRESS);//<-<- PARTE A MODIFICAR >->->
        address=ControlDeCarro.direcc;
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
                btSocket.close();
            } catch (IOException e2) {}
        }
        MyConexionBT = new Guardar.ConnectedThread(btSocket);
        MyConexionBT.start();
    }

    @Override
    public void onPause()
    {
        super.onPause();
        try
        {
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

            while (true) {
                try {
                    bytes = mmInStream.read(buffer);            //read bytes from input buffer
                    String readMessage = new String(buffer, 0, bytes);
                    // Send the obtained bytes to the UI Activity via handler

                    Lectura=Lectura+readMessage;
                    if(readMessage.contains("$")){
                        CaraInfo(Lectura);

                        Lectura="";

                    }
                    //if(read)
                } catch (IOException e) {
                    break;
                }
            }




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
                Log.d("Error", String.valueOf(e));

                //finish();
            }
        }

    }
}
