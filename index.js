language = localStorage.getItem("language") || "en";
nowIn = localStorage.getItem("nowIn") || "home";
window.location.replace(`${language}/${nowIn}.html`);//导向