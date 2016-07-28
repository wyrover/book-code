<!DOCTYPE DCF>

<assistantconfig version="3.2.0">

<profile>
  <property name="name">qtbookexample</property>
  <property name="title">Qt Book Example</property>
  <property name="applicationicon">images/qt.png</property>
  <property name="startpage">index.html</property>
  <property name="aboutmenutext">About The Qt Book Example</property>
  <property name="abouturl">about.txt</property>
  <property name="assistantdocs">.</property>
</profile>

<DCF ref="index.html" icon="images/qt.png" title="Qt Book Example">
  <section ref="./basics.html" title="Basics">
    <section ref="./index.html" title="The first basic thing" />
    <section ref="./index.html" title="The second basic thing" />
    <section ref="./easystuff.html" title="Another basic topic" />

    <keyword ref="./index.html">Basic Thing One</keyword>
    <keyword ref="./index.html">Basic Thing Two</keyword>
    <keyword ref="./easystuff.html">Another Basic Thing</keyword>
  </section>
  <section ref="./advanced.html" title="Advanced Topics">
    <section ref="./adv1.html" title="The first advanced thing" />
    <section ref="./adv2.html" title="The second advanced thing" />

    <keyword ref="./adv1.html">Advanced Topic One</keyword>
    <keyword ref="./adv2.html">Advanced Topic Two</keyword>
  </section>
    
  <section ref="./appendix.html" title="Appendix" />
  <section ref="./faq.html" title="F.A.Q." />
</DCF>

</assistantconfig>