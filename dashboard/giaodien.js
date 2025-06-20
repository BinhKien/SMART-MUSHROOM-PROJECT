// Cấu hình Firebase
const firebaseConfig = {
  apiKey: "AIzaSyBzRnhaMbxUYEP075vS8p1iUw2jYH5CPdA",
  authDomain: "doan1-21fc2.firebaseapp.com",
  databaseURL: "https://doan1-21fc2-default-rtdb.firebaseio.com",
  projectId: "doan1-21fc2",
  storageBucket: "doan1-21fc2.appspot.com",
  messagingSenderId: "194635143597",
  appId: "1:194635143597:web:3be06fdbb3a88f41576924",
  measurementId: "G-8V23G4FNFG"
};

// Khởi tạo Firebase
firebase.initializeApp(firebaseConfig);
const database = firebase.database();

// DOM Elements
const nhietdoEl = document.getElementById("nhietdo");
const doamEl = document.getElementById("doam");
const anhsangEl = document.getElementById("anhsang");

const tuoiNuocEl = document.getElementById("tuoinuoc");
const trangThaiDenEl = document.getElementById("trangthaid");
const manCheEl = document.getElementById("manche");

// Lắng nghe dữ liệu
database.ref().on("value", (snapshot) => {
  const data = snapshot.val();

  const nhietdo = parseFloat(data?.nhietdo);
  const doam = parseFloat(data?.doam);
  const anhsang = parseFloat(data?.anhsang);

  nhietdoEl.innerText = !isNaN(nhietdo) ? nhietdo + " °C" : "-- °C";
  doamEl.innerText    = !isNaN(doam) ? doam + " %" : "-- %";
  anhsangEl.innerText = !isNaN(anhsang) ? anhsang + " Lux" : "-- Lux";

  // Tưới nước: nếu độ ẩm < 80 và nhiệt độ > 35 thì hoạt động
  if (!isNaN(doam) && !isNaN(nhietdo) && doam < 80 && nhietdo > 34) {
    tuoiNuocEl.innerText = "Đang hoạt động";
  } else {
    tuoiNuocEl.innerText = "Tắt";
  }

  // Màn che: nếu ánh sáng > 800 thì đóng
  let trangThaiManChe = "--";
  if (!isNaN(anhsang)) {
    trangThaiManChe = anhsang > 500 ? "Đóng" : "Mở";
  }
  manCheEl.innerText = trangThaiManChe;

  // Đèn: nếu màn che đang đóng thì bật
  if (trangThaiManChe === "Mở") {
    trangThaiDenEl.innerText = "Bật";
  } else if (trangThaiManChe === "Đóng") {
    trangThaiDenEl.innerText = "Tắt";
  } else {
    trangThaiDenEl.innerText = "--";
  }
});
