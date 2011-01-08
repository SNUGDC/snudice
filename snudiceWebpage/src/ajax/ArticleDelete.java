package ajax;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import utility.Util;

import beans.Article;
import dbaccess.DB;

/**
 * Servlet implementation class ArticleDelete
 */
@WebServlet("/ArticleDelete")
public class ArticleDelete extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public ArticleDelete() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		HttpSession session = request.getSession();
		String userId = (String)session.getAttribute("userId");				
		
		int articleIndex = Integer.parseInt(request.getParameter("articleIndex"));
		
		DB db = DB.getInstance();
		Article article= db.dbBoard.getArticleByIndex(articleIndex);
	
		if(article.getUserId().compareTo(userId)==0) //아이디가 글쓴이와 일치하면
		{			
			//글에 달린 리플들을 지운다.
			db.dbBoard.deleteReply(articleIndex);
			//글을 지운다.		
			db.dbBoard.deleteArticle(articleIndex);
		}
	}
}