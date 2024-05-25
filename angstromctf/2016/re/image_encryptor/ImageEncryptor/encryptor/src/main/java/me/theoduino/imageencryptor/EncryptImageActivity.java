package me.theoduino.imageencryptor;

import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.*;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Random;


public class EncryptImageActivity extends ActionBarActivity {
    Uri file;
    final int READ_REQUEST_CODE = 42;
    private static final String TAG = "EncryptImageActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_encrypt_image);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_encrypt_image, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == READ_REQUEST_CODE && resultCode == RESULT_OK && data != null) {
            file = data.getData();
            Log.d(TAG, "Uri: " + file.toString());
            TextView textView = (TextView)findViewById(R.id.textview_uri);
            textView.setText(file.toString());
        }
    }

    public void onSelectFileClicked(View view) {
        final Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("image/*");
        startActivityForResult(intent, READ_REQUEST_CODE);
    }

    public void onEncryptClicked(View view) {
        EditText passwordInput = (EditText) findViewById(R.id.text_password);
        String password = passwordInput.getText().toString();
        if(password.equals("")) {
            Toast.makeText(getApplicationContext(), "You must enter a password!", Toast.LENGTH_SHORT).show();
            return;
        }
        byte[] key = md5(password);
        if(file.toString().length() == 0) {
            Toast.makeText(getApplicationContext(), "You must select a file!", Toast.LENGTH_SHORT).show();
            return;
        }

        byte[] cipherText;
        try {
            cipherText = encryptData(readUri(file), key);
        } catch(IOException e) {
            Toast.makeText(getApplicationContext(), "Could not read file", Toast.LENGTH_SHORT).show();
            return;
        }

        File outputFile;
        try {
            outputFile = new File(getOutputPath(), generateRandomFilename(8) + ".encrypted");
        } catch(IOException e) {
            Log.e(TAG, "Unable to connect to external storage");
            return;
        }

        try {
            FileOutputStream fileOutputStream = new FileOutputStream(outputFile);
            fileOutputStream.write(cipherText);
            fileOutputStream.close();
        } catch (FileNotFoundException e) {
            Log.e(TAG, "Could not find output directory");
            return;
        } catch(IOException e) {
            Log.e(TAG, "Unable to write to file");
            return;
        }

        Toast.makeText(getApplicationContext(), "Successfully created file " + outputFile.getAbsolutePath(), Toast.LENGTH_LONG).show();


    }

    private File getOutputPath() throws IOException {
        String string = Environment.getExternalStorageState();
        if(!Environment.MEDIA_MOUNTED.equals(string)) {
            throw new IOException("Error connecting to external media");
        }
        File file = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/encryptimage/");
        file.mkdir();
        return file;
    }

    private String generateRandomFilename(int length) {
        Random random = new Random();
        StringBuilder stringBuilder = new StringBuilder(length);
        char temp;
        for(int i = 0; i < length; i++) {
            temp = (char)random.nextInt(62);
            if(temp < 26) {
                temp += 'a';
            } else if(temp < 52) {
                temp += 'A';
            } else {
                temp += '0';
            }
            stringBuilder.append(temp);
        }
        return stringBuilder.toString();
    }

    private byte[] readUri(Uri uri) throws IOException {
        InputStream inputStream = getContentResolver().openInputStream(uri);

        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        byte[] buffer = new byte[1024];
        int read;
        while(true) {
            read = inputStream.read(buffer);
            if(read == -1) {
                break;
            }
            byteArrayOutputStream.write(buffer, 0, read);
        }

        return byteArrayOutputStream.toByteArray();
    }

    private byte[] md5(String data) {
        try {
            MessageDigest messageDigest = MessageDigest.getInstance("md5");
            messageDigest.update(data.getBytes());
            return messageDigest.digest();
        } catch(NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        return new byte[0];
    }

    private byte[] encryptData(byte[] data, byte[] key) {
        byte keyLength = (byte)key.length;
        int dataLength = data.length;
        byte[] cipherText = new byte[dataLength];
        for(int i = 0; i < data.length; i++) {
            cipherText[i] = (byte)(data[i] ^ key[i % keyLength]);
        }
        return cipherText;
    }
}
