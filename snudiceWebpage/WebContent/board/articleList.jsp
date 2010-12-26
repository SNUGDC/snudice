<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib prefix ="c" uri="http://java.sun.com/jsp/jstl/core" %>
    
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="/snudiceWebpage/css/boardStyle.css"/>
<title>${sessionScope.boardName}게시판</title>
</head>
<body>	
	<table class="boardList">	
		<tr>
			<td class="articleNumber">번호</td>
			<td class="articleTitle">제목</td>
			<td class="articleWriter">글쓴이</td>
			<td class="articleDateTime">날짜</td>
			<td class="articleReadCount">조회수</td>
		</tr>
		
		<c:forEach var="article" items="${articleList}">	
		<tr>
			<td>${article.articleIndex}</td>
			<td><a href="/snudiceWebpage/articleView.do?articleIndex=${article.articleIndex}">${article.title}</a></td>
			<td>${article.userId}</td>
			<td>${article.dateTime}</td> 
			<td>${article.readCount}</td>		
		</tr>
		</c:forEach>
		</table>
		<table class="boardList">
		<tr>
		<td class="pageNumber"> <a href="/snudiceWebpage/articleList.do?goPrev=true&currPage=${currPage}">prev</a> </td>
		<c:forEach var="pageNumber" items="${pageList}">
			<td class="pageNumber">
			
			<c:choose>
				<c:when test="${pageNumber==currPage}">
					<a href="/snudiceWebpage/articleList.do?currPage=${pageNumber}"> (${pageNumber})</a>
				</c:when>
			<c:otherwise>
			<a href="/snudiceWebpage/articleList.do?currPage=${pageNumber}"> ${pageNumber}</a>
			</c:otherwise>
			</c:choose>
			</td>
		</c:forEach>
		<td class="pageNumber"> <a href="/snudiceWebpage/articleList.do?currPage=${currPage}&goNext=true">next</a> </td>
		</tr>
		</table>
	현재 페이지 : ${sessionScope.currPage}  		
	<br/>
	<a href="/snudiceWebpage/articleList.do?currPage=0">처음목록</a>
	<a href="/snudiceWebpage/main.do">메인으로</a>
	
	<a id="boardWrite" href="/snudiceWebpage/board/articleWrite.jsp?boardName=${sessionScope.boardName}">글쓰기</a>
</body>
</html>