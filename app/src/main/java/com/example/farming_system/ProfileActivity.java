package com.example.farming_system;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.Map;

public class ProfileActivity extends AppCompatActivity {
    TextView profileName, profileEmail, profileUsername, profilePassword,tvMoisture,tvPressure,tvhumidity,tvtemp;
    TextView titleName, titleUsername;
    Button editProfile, editButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_profile);

        profileName = findViewById(R.id.profileName);
        profileEmail = findViewById(R.id.profileEmail);
        profileUsername = findViewById(R.id.profileUsername);
        profilePassword = findViewById(R.id.profilePassword);
        titleName = findViewById(R.id.titleName);
        titleUsername = findViewById(R.id.titleUsername);
        editProfile = findViewById(R.id.editButton);
        tvMoisture=findViewById(R.id.moistureNo);
        tvPressure = findViewById(R.id.pressureNo);
        tvhumidity = findViewById(R.id.HumidityNo);
        tvtemp =findViewById(R.id.tempNo);
        editButton = findViewById(R.id.editButton);

        showAllUserData();

        editButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(ProfileActivity.this,popup_MainActivity.class);
                startActivity(intent);

            }
        });
    }


    public void showAllUserData(){
        Intent intent = getIntent();
        String nameUser = intent.getStringExtra("name");
        String emailUser = intent.getStringExtra("email");
        String usernameUser = intent.getStringExtra("username");
        String passwordUser = intent.getStringExtra("password");
        String moistureData=intent.getStringExtra("moistureValue");
        String tempData=intent.getStringExtra("tempValue");
        String pressureData=intent.getStringExtra("pressureValue");
        String humidityData=intent.getStringExtra("HumidityValue");

        titleName.setText(nameUser);
        titleUsername.setText(usernameUser);
        profileName.setText(nameUser);
        profileEmail.setText(emailUser);
        profileUsername.setText(usernameUser);
        profilePassword.setText(passwordUser);
        tvMoisture.setText(moistureData);
        tvtemp.setText(tempData);
        tvhumidity.setText(humidityData);
        tvPressure.setText(pressureData);
    }



}
