package com.example.farming_system;

import static androidx.constraintlayout.helper.widget.MotionEffect.TAG;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.Query;
import com.google.firebase.database.ValueEventListener;

public class LoginActivity extends AppCompatActivity {

    EditText loginUsername, loginPassword;
    Button loginButton;
    TextView signupRedirectText;
    TextView moist;
    String moistureString;
    String nameFromDB;
    String passwordFromDB;
    String emailFromDB;
    String usernameFromDB;
    String tempString;

    String preString;
    String HumString;


    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        loginUsername = findViewById(R.id.login_username);
        loginPassword = findViewById(R.id.login_password);
        loginButton = findViewById(R.id.login_button);
        signupRedirectText = findViewById(R.id.signupRedirectText);

    loginButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                if (!validateUsername() | !validatePassword()) {
                } else {
                    checkUser();
                    sendMoistureData();
                    sendHumData();
                    sendPreData();
                    sendTempData();
                }
            }

        });
        signupRedirectText.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(LoginActivity.this, SignupActivity.class);
                startActivity(intent);
            }
        });


    }

    public void callingIntent(){
        Intent intent = new Intent(LoginActivity.this, ProfileActivity.class);

        intent.putExtra("name", nameFromDB);
        intent.putExtra("email", emailFromDB);
        intent.putExtra("username", usernameFromDB);
        intent.putExtra("password", passwordFromDB);

        intent.putExtra("moistureValue",moistureString);
        intent.putExtra("pressureValue",preString);
        intent.putExtra("HumidityValue",HumString);
        intent.putExtra("tempValue",tempString);

        startActivity(intent);
    }

public void sendMoistureData() {
    DatabaseReference moistureRef = FirebaseDatabase.getInstance().getReference("test");
    Query userRef = moistureRef.child("array").child("3");

    userRef.addListenerForSingleValueEvent(new ValueEventListener() {
        @Override
        public void onDataChange(DataSnapshot dataSnapshot) {
            Double moisture = dataSnapshot.getValue(Double.class);
            moistureString = moisture.toString();
            callingIntent();
        }


        @Override
        public void onCancelled(DatabaseError databaseError) {
            // Handle error
        }
    });
}
    public void sendTempData() {
        DatabaseReference moistureRef = FirebaseDatabase.getInstance().getReference("test");
        Query userRef = moistureRef.child("array").child("0");

        userRef.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                Double temp = dataSnapshot.getValue(Double.class);
                tempString = temp.toString();
                Toast.makeText(LoginActivity.this, tempString, Toast.LENGTH_SHORT).show();

                callingIntent();
            }


            @Override
            public void onCancelled(DatabaseError databaseError) {
                // Handle error
            }
        });
    }


    public void sendPreData() {
        DatabaseReference moistureRef = FirebaseDatabase.getInstance().getReference("test");
        Query userRef = moistureRef.child("array").child("2");

        userRef.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                Double pre = dataSnapshot.getValue(Double.class);
                preString = pre.toString();
                callingIntent();
            }


            @Override
            public void onCancelled(DatabaseError databaseError) {
                // Handle error
            }
        });
    }
    public void sendHumData() {
        DatabaseReference moistureRef = FirebaseDatabase.getInstance().getReference("test");
        Query userRef = moistureRef.child("array").child("1");

        userRef.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                Double Hum = dataSnapshot.getValue(Double.class);
            HumString = Hum.toString();
                callingIntent();
            }


            @Override
            public void onCancelled(DatabaseError databaseError) {
                // Handle error
            }
        });
    }
    public Boolean validateUsername(){
        String val = loginUsername.getText().toString();
        if(val.isEmpty()){
            loginUsername.setError("Username cannot be empty");
            return false;

        }
        else {
            loginUsername.setError(null);
            return true;
        }
    }
    public Boolean validatePassword(){
        String val = loginPassword.getText().toString();
        if (val.isEmpty()) {
            loginPassword.setError("Password cannot be empty");
            return false;
        } else {
            loginPassword.setError(null);
            return true;
        }
    }
    public void checkUser(){
        String userUsername = loginUsername.getText().toString().trim();
        String userPassword = loginPassword.getText().toString().trim();

        DatabaseReference reference = FirebaseDatabase.getInstance().getReference("users");
        Query checkUserDatabase = reference.orderByChild("username").equalTo(userUsername);

        checkUserDatabase.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {

                if(snapshot.exists()){
                    loginUsername.setError(null);
                    passwordFromDB = snapshot.child(userUsername).child("password").getValue(String.class);

                    if(passwordFromDB.equals(userPassword)){
                        loginUsername.setError(null);

                        nameFromDB = snapshot.child(userUsername).child("name").getValue(String.class);
                        emailFromDB = snapshot.child(userUsername).child("email").getValue(String.class);
                        usernameFromDB = snapshot.child(userUsername).child("username").getValue(String.class);

                        callingIntent();

                    }else {
                        loginPassword.setError("Invalid Credentials");
                        loginPassword.requestFocus();
                    }
                }else {
                    loginUsername.setError("User does not exist");
                    loginUsername.requestFocus();
                }

            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });



    }
}