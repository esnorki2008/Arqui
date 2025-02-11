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
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import org.w3c.dom.Text;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.UUID;

public class Guardar extends AppCompatActivity {

    //1)
    Button BtnRecorrer, BtnLimpiar,BtnActualizar;
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

    ArrayList Titulo;
    ArrayList Arr;
    ArrayList MegaAuxiliar;
    String ValorContenido="";
    String Matriz[][] = new String[3][20];
    private  ArrayAdapter Adapt,AdaptSuperior;
//-------------------------------------------

    private void CaraInfo(String Dita){
        Arr.clear();
        Titulo.clear();
        MegaAuxiliar=new ArrayList();

        String[] Ru=Dita.split("\\}");
        for(int i=0;i<3;i++){


            String[] Contend=Ru[i].split(",");
            //
            if(Contend[0].equals("nullp")){
                Titulo.add("RutaLibre");
            }else{
                Titulo.add(Contend[0]);
            }
            String Value="";
            for(int j=1;j<21;j++){
                Matriz[i][j-1]=(Contend[j]);
                if(j%2==0){//Llenar Array Con Valores

                    if(!Contend[j].equals("0")) {
                        Value = Value + "Tiempo: " + Contend[j];
                        Arr.add(Value);
                        MegaAuxiliar.add(Value);
                    }else{
                        MegaAuxiliar.add(Value);
                        Arr.add(Value);
                    }
                    Value="";
                }else{
                    Value=Value+"Direccion: "+Contend[j]+"     ";
                }
            }

        }
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_guardar);
        //2)
        ListaSuperior=(ListView)findViewById(R.id.LstRutas);
        ListaInferior=(ListView)findViewById(R.id.LstContenido);
        BtnLimpiar = (Button) findViewById(R.id.BtnLimpiar);
        BtnRecorrer = (Button) findViewById(R.id.BtnRecorrer);
        BtnActualizar=(Button) findViewById(R.id.BtnActualizar);
        Seleccionado= (TextView)findViewById(R.id.LblRutas);


        Titulo= new ArrayList();
        AdaptSuperior= new ArrayAdapter(this,android.R.layout.simple_list_item_1,Titulo);
        ListaSuperior.setAdapter(AdaptSuperior);
        ListaSuperior.setOnItemClickListener(mDeviceClickListener);


        Arr=new ArrayList();
        Adapt= new ArrayAdapter(this,android.R.layout.simple_list_item_1,Arr);
        ListaInferior.setAdapter(Adapt);

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

        BtnLimpiar.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
               Borrar();
            }
        });
        BtnActualizar.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {

                Seleccionado.setText("");
                InfoEnvia("i");
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Adapt.notifyDataSetChanged();
               AdaptSuperior.notifyDataSetChanged();
            }
        });
        BtnRecorrer.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
               Recorrer();
            }
        });





    }

    private AdapterView.OnItemClickListener mDeviceClickListener = new AdapterView.OnItemClickListener() {
        public void onItemClick(AdapterView av, View v, int arg2, long arg3) {
            String info = ((TextView) v).getText()+"";
            Seleccionado.setText(info);
            //Seleccionado.setText(info);
            Arr.clear();
            for(int i=0;i<3;i++){
                if(info.toLowerCase().trim().equals(Titulo.get(i).toString().toLowerCase().trim())){
                    Log.d("OPARS","OCURRIORIOI      "+Arr.size()+"      "+i );
                    for(int j=0;j<MegaAuxiliar.size();j++){
                        if(j/10==i){
                            Arr.add(MegaAuxiliar.get(j));
                        }
                    }


                }
            }


            Adapt.notifyDataSetChanged();
        }
    };

    void Borrar(){//Eliminar Algún Struct
        String info = Seleccionado.getText().toString();
        if(info.toLowerCase().trim().equals("rutalibre")){
            Toast.makeText(getBaseContext(), "Una Ruta Libre No Se Puede Recorrer", Toast.LENGTH_LONG).show();
            return;
        }
        //Seleccionado.setText(info);
        ArrayList Aux = new ArrayList();
        for(int i=0;i<3;i++){
            if(info.toLowerCase().trim().equals(Titulo.get(i).toString().toLowerCase().trim())){
                InfoEnvia("b");
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                InfoEnvia(i+"");
                return;
            }
        }
    }
    void Recorrer(){//Recorrer Arreglos
        String info = Seleccionado.getText().toString();
        if(info.toLowerCase().trim().equals("rutalibre")){
            Toast.makeText(getBaseContext(), "Una Ruta Libre No Se Puede Limpiar", Toast.LENGTH_LONG).show();
            return;
        }

        for(int i=0;i<3;i++){
            if(info.toLowerCase().trim().equals(Titulo.get(i).toString().toLowerCase().trim())){
                InfoEnvia("r");
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                for(int j=0;j<20;j++){//Enviar Info
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    InfoEnvia(Matriz[i][j]);
                    InfoEnvia(",");
                }
                return;
            }
        }
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

        //address = intent.getStringExtra(DispositivosBT.EXTRA_DEVICE_ADDRESS);//<-<- PARTE A MODIFICAR >->->
        address=DispositivosBT.Direcc;
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
                Log.d("Conex",btSocket.toString());
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
                        //TxtContenido.setText(ValorContenido);

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
