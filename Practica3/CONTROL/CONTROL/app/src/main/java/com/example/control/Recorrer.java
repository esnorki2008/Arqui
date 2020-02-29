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
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.UUID;

public class Recorrer extends AppCompatActivity {

    //1)
    Button BtnGuardar,BtnAgregar;
    Spinner Spi1,Spi2;
    ListView LstElementos;
    TextView TxtIngreso;
    ArrayAdapter Master;
    ArrayList MasterDetalle;
    //-------------------------------------------
    Handler bluetoothIn;
    final int handlerState = 0;
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private StringBuilder DataStringIN = new StringBuilder();
    private Recorrer.ConnectedThread MyConexionBT;
    // Identificador unico de servicio - SPP UUID
    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    // String para la direccion MAC
    private static String address = null;
    int Contador=0;
    byte[] NuevaRuta = new byte[20];
//-------------------------------------------

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_recorrer);
        //2)


        BtnGuardar = (Button) findViewById(R.id.BtnGuardar);
        BtnAgregar = (Button) findViewById(R.id.BtnAgregar);
        LstElementos=(ListView)findViewById(R.id.LstContenido);
        TxtIngreso=(TextView)findViewById(R.id.TxtIngreso);
        Spi1=(Spinner)findViewById(R.id.Spi1);
        Spi2=(Spinner)findViewById(R.id.Spi2);
        MasterDetalle=new ArrayList();
        ArrayList Movimientos= new ArrayList();
        Movimientos.add("arriba");
        Movimientos.add("abajo");
        Movimientos.add("izquierda");
        Movimientos.add("derecha");


        ArrayList Tiempos= new ArrayList();
        for(int i=1;i<21;i++){
            Tiempos.add(i+"");
        }
        ArrayAdapter Adapta1 = new ArrayAdapter(this,android.R.layout.simple_list_item_1,Movimientos);
        ArrayAdapter Adapta2 = new ArrayAdapter(this,android.R.layout.simple_list_item_1,Tiempos);
        Master= new ArrayAdapter(this,android.R.layout.simple_list_item_1,MasterDetalle);

        LstElementos.setAdapter(Master);
        Spi1.setAdapter(Adapta1);
        Spi2.setAdapter(Adapta2);

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
        BtnAgregar.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                if(Contador<=18){
                String Direccion=Spi1.getSelectedItem().toString();
                String Tiempo=Spi2.getSelectedItem().toString();
                MasterDetalle.add("Direccion: "+Direccion+", Tiempo: "+Tiempo);
                Master.notifyDataSetChanged();
                byte Direc=0;
                switch (Direccion.toLowerCase().trim()){
                    case "arriba":
                        Direc=0;
                        break;
                    case "abajo":
                        Direc=1;
                        break;
                    case "izquierda":
                        Direc=2;
                        break;
                    case "derecha":
                        Direc=3;
                        break;
                }


                    NuevaRuta[Contador]=Direc;
                    NuevaRuta[Contador+1]=Byte.parseByte(Tiempo);
                    Contador=Contador+2;
                }
            }
        });

        BtnGuardar.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                if(MasterDetalle.size()==0) {
                    Toast.makeText(getBaseContext(), "No Se Ha Agregado Nada Nuevo A La Ruta", Toast.LENGTH_LONG).show();
                    return;
                }
                InfoEnvia("i");
                int AntiBucle=0;
                while(Titulo==null){
                    AntiBucle++;
                    if(AntiBucle>10){
                        break;
                    }
                try {
                    Thread.sleep(2000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                }


                int Index=2;
                for(int i=0;i<3;i++){
                    if(Titulo.get(i).toString().toLowerCase().trim().equals("rutalibre")){

                        Index=i;
                        break;
                    }
                }
                ArrayList Tempo = new ArrayList();
                for(int i=0;i<3;i++){
                    if(i!=Index){
                        Tempo.add(Titulo.get(i).toString());
                    }else{
                        Tempo.add(TxtIngreso.getText());
                    }
                }
                Titulo=Tempo;
                for(int j=0;j<20;j++){

                        Matriz[Index][j]=NuevaRuta[j]+"";
                        NuevaRuta[j]=0;
                }
                TxtIngreso.setText("");
                MasterDetalle.clear();
                Master.notifyDataSetChanged();
                EnviarTodo();
            }
        });
    }

    private void EnviarTodo(){
        for(int i=0;i<3;i++){

            InfoEnvia("z");
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            String Mando=Titulo.get(i).toString();
            if(Mando.toLowerCase().trim().equals("rutalibre")){Mando="nullp";}
            if(Mando.length()>5){Mando=Mando.substring(0,5);}

            InfoEnvia(Mando);
            try {
                Thread.sleep(1000);
                InfoEnvia(",");
                Thread.sleep(100);
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
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        }

    }
    ArrayList MegaAuxiliar;
    ArrayList Titulo;
    String Matriz[][] = new String[3][20];
    private void CaraInfo(String Dita){

        Titulo= new ArrayList();
        MegaAuxiliar=new ArrayList();

        String[] Ru=Dita.split("\\}");
        for(int i=0;i<3;i++){


            String[] Contend=Ru[i].split(",");
            //
            if(Contend[0].equals("nullp")){
                Titulo.add("rutalibre");
            }else{
                Titulo.add(Contend[0]);
            }

            for(int j=1;j<21;j++){
                Matriz[i][j-1]=(Contend[j]);

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
        //Setea la direccion MAC
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
        MyConexionBT = new Recorrer.ConnectedThread(btSocket);
        MyConexionBT.start();
    }

    @Override
    public void onPause()
    {
        super.onPause();
        try
        { // Cuando se sale de la aplicación esta parte permite
            // que no se deje abierto el socket
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
