<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="${root}/css/loginPageStyle.css"/>
<title>로그인 페이지</title>
</head>
<body>
	<div class="wrapper">
		<div class="header" id="loginHeader">
			<img src="${root}/image/mainpage/title.png"/>
		</div>
		<div class="container" id="loginContainer">
			<div class="left">
				<img src="${root}/image/mainpage/loginWindow.png" />
				* <a class="highlight" href="${root}/join.jsp">회원가입</a>
	        	| <a class="highlight" href="${root}/notimplemented.jsp">아이디, 비밀번호찾기</a>
	        	<a href="logout.do">로그아웃</a>
			</div>
			<div class="center" id="loginCenter">
				<jsp:include page="loginForm.jsp"/>			
				<a href="${root}/clientFile/update/6001/a.txt">클라이언트 다운로드</a>	
			</div>
			<div class="right">&nbsp;</div>	
		</div>
		<div class="footer" id="loginFooter">		
		</div>			
	</div>
</body>
</html>