<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema">
<xsl:template match="/">
<html>
<head />
  <body title="">
     <xsl:for-each select="unittest-results">
     <h1>Test Report</h1>
     <p>
     <table border="0">
          <thead>
          <tr>
                    <td>Total number of tests</td>
                    <td>Number of failed tests</td>
                    <td>Failures</td>
                    <td>Time</td>
          </tr>
       </thead>
       <tbody>

        <tr>
        <td>
        <xsl:for-each select="@tests">
        <xsl:value-of select="." />
        </xsl:for-each>
        </td>

        <td>
        <xsl:for-each select="@failedtests">
        <xsl:value-of select="." />
        </xsl:for-each>
        </td>
        
        <td>
        <xsl:for-each select="@failures">
        <xsl:value-of select="." />
        </xsl:for-each>
        </td>

        <td>
        <xsl:for-each select="@failures">
        <xsl:value-of select="." />
        </xsl:for-each>
        </td>
        </tr>
       </tbody>
       </table>

    <br/>
     <xsl:for-each select="test">
     <xsl:if test="position( )=1">
     <h2>Test Table</h2>
          <table border="0" cellspacing="5" >
          <thead>
          <tr>
                    <td>Suite</td>
                    <td>Name</td>
                    <td>Time</td>
                    <td>Status</td>
                    <td>Info</td>
          </tr>
       </thead>
       <tbody>
       <xsl:for-each select="../test">
           <tr>
           <td >
              <xsl:for-each select="@suite">
              <xsl:value-of select="." />
              </xsl:for-each>
         </td>
         <td >
              <xsl:for-each select="@name">
              <xsl:value-of select="." />
              </xsl:for-each>
        </td>
        <td >
              <xsl:for-each select="@time">
              <xsl:value-of select="." />
              </xsl:for-each>
        </td>
        <td >
            <xsl:if test="failure">
            FAIL
            </xsl:if>
            <xsl:if test="not(failure)">
            PASS
            </xsl:if>
        </td>
         <td>
            <xsl:if test="failure">
            <xsl:for-each select="failure">
              <xsl:for-each select="@message">
              <xsl:value-of select="." />
              </xsl:for-each>
              <br/>
            </xsl:for-each>
            </xsl:if>
        </td>
        </tr>
        </xsl:for-each>
      </tbody>
      </table>
      </xsl:if>
</xsl:for-each>
</p>
</xsl:for-each>
</body>
</html>
</xsl:template>
</xsl:stylesheet>

